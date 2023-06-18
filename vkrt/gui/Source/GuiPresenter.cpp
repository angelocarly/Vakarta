//
// Created by magnias on 6/18/23.
//

#include <filesystem>
#include <spdlog/spdlog.h>
#include "vkrt/gui/GuiPresenter.h"

#include "vks/render/Device.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

static std::shared_ptr< vkrt::GuiPresenter > mInstance;

std::shared_ptr< vkrt::GuiPresenter >
vkrt::GuiPresenter::GetInstance()
{
    return mInstance;
}

void
vkrt::GuiPresenter::Initialize( vks::DevicePtr inDevice, vks::SwapchainPtr inSwapchain, vks::WindowPtr inWindow )
{
    if( mInstance )
    {
        throw std::runtime_error( "GuiPresenter already initialized" );
    }
    mInstance = std::unique_ptr< GuiPresenter >( new GuiPresenter( inDevice, inSwapchain, inWindow ) );
}

vkrt::GuiPresenter::GuiPresenter( vks::DevicePtr inDevice, vks::SwapchainPtr inSwapchain, vks::WindowPtr inWindow )
:
    Presenter( inDevice, inSwapchain->GetExtent().width, inSwapchain->GetExtent().height ),
    mDevice( inDevice ),
    mSwapChain( inSwapchain ),
    mWindow( inWindow )
{
    InitializeDescriptorPool();
    InitializeImGui();
}

vkrt::GuiPresenter::~GuiPresenter()
{
    mDevice->GetVkDevice().waitIdle();

    mDevice->GetVkDevice().destroy( mDescriptorPool );

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ::ImGui::DestroyContext();
}

void
vkrt::GuiPresenter::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{
    {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigDockingWithShift = true;

        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();

        // Demo window
        ImGui::ShowDemoWindow();

        if (ImGui::BeginMainMenuBar())
        {
            if( ImGui::BeginMenu( "Menu" ) )
            {
//                ImGui::MenuItem("Show stats" , nullptr, &mShowStats );
//                ImGui::MenuItem("Show tools" , nullptr, &mShowTools );
//                ImGui::MenuItem("Show demo window" , nullptr, &mShowDemoWindow );
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Render();
    }

    BeginRenderPass( inRenderEnvironment.mCommandBuffer );
    {
        ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), inRenderEnvironment.mCommandBuffer );
    }
    EndRenderPass( inRenderEnvironment.mCommandBuffer );
}

// ============================================= Initialization ========================================================

void
vkrt::GuiPresenter::CheckVkResult( VkResult inResult )
{
    if ( inResult == 0 ) return;

    spdlog::error( "[ImGui][Vulkan] Error: VkResult = {}", inResult );

    if ( inResult < 0) abort();
}

void
vkrt::GuiPresenter::InitializeImGui()
{
    ::IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();
    (void) io;

    SetImGuiStyle();

    // Load fonts
    // TODO: Load fonts dynamically
    auto fontPath = std::filesystem::path( "./" ) / "resources" / "fonts" / "Hack-Regular.ttf";
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF( fontPath.string().c_str(), 16.0f );

    // Setup Vulkan bindings
    ImGui_ImplGlfw_InitForVulkan( mWindow->GetGLFWWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vks::Instance::GetInstance().GetVkInstance();
    init_info.PhysicalDevice = mDevice->GetPhysicalDevice()->GetVkPhysicalDevice();
    init_info.Device = mDevice->GetVkDevice();
    init_info.QueueFamily = mDevice->GetPhysicalDevice()->FindQueueFamilyIndices().graphicsFamilyIndex.value();
    init_info.Queue = mDevice->GetVkQueue();
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = mDescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = mSwapChain->GetMinImageCount();
    init_info.ImageCount = mSwapChain->GetImageCount();
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = CheckVkResult;
    ImGui_ImplVulkan_Init( &init_info, GetRenderPass() );

    // Create fonts
    vk::CommandBuffer theCommandBuffer = mDevice->BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(theCommandBuffer );
    mDevice->EndSingleTimeCommands( theCommandBuffer );
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void
vkrt::GuiPresenter::InitializeDescriptorPool()
{
    std::vector< vk::DescriptorPoolSize > thePoolSizes =
        {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageBuffer, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };

    vk::DescriptorPoolCreateInfo const theDescriptorPoolCreateInfo
        (
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            1000, // Correct?
            thePoolSizes.size(),
            thePoolSizes.data()
        );

    mDescriptorPool = mDevice->GetVkDevice().createDescriptorPool( theDescriptorPoolCreateInfo );
}

void
vkrt::GuiPresenter::SetImGuiStyle()
{
    ImGui::StyleColorsDark();
    ImGuiStyle* theStyle = &ImGui::GetStyle();
    theStyle->WindowBorderSize = 0.0f;
}

