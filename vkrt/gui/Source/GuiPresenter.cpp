//
// Created by magnias on 6/18/23.
//

#include "vkrt/gui/GuiPresenter.h"

#include "vkrt/gui/GuiDrawer.h"

#include "vks/render/Device.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"
#include "vks/render/Utils.h"

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

#include <filesystem>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3native.h>

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

void
vkrt::GuiPresenter::Finalize()
{
    mInstance.reset();
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

        ImGui::SetNextWindowBgAlpha( 0.0f );
        ImGui::DockSpaceOverViewport( nullptr, ImGuiDockNodeFlags_PassthruCentralNode );

        for( auto & drawer : mGuiDrawers )
        {
            drawer.lock()->DrawGui();
        }

        ImGui::Render();
    }

    BeginPresenterRenderPass( inRenderEnvironment.mCommandBuffer );
    {
        ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), inRenderEnvironment.mCommandBuffer );
    }
    EndPresenterRenderPass( inRenderEnvironment.mCommandBuffer );
}

void
vkrt::GuiPresenter::RegisterGuiDrawer( std::weak_ptr< vkrt::gui::GuiDrawer > inGuiDrawer )
{
    mGuiDrawers.push_back( inGuiDrawer );
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

    SetImGuiStyle( io );

    // Load fonts
    // TODO: Load fonts dynamically
    auto fontPath = std::filesystem::path( "./" ) / "resources" / "fonts" / "Hack-Regular.ttf";
    io.Fonts->AddFontDefault();
//    io.Fonts->AddFontFromFileTTF( fontPath.string().c_str(), 16.0f );

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
vkrt::GuiPresenter::SetImGuiStyle( ImGuiIO & io )
{
    ImGui::StyleColorsDark();
    ImGuiStyle* theStyle = &ImGui::GetStyle();
    theStyle->WindowBorderSize = 0.0f;
    theStyle->WindowPadding = ImVec2( 4, 4 );
    theStyle->FramePadding = ImVec2( 4, 2 );
    theStyle->ItemSpacing = ImVec2( 8, 4 );

    // Double the scale on Mac displays as ImGui doesn't provide appropriate font sizes for them.
    // https://github.com/ocornut/imgui/issues/5301
    if( vks::Utils::IsTargetApple() )
    {
        float scale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13.0f * scale;
        io.Fonts->AddFontDefault( & cfg );
        io.FontGlobalScale = 1.0f / scale;
    }

    ImVec4 theBaseColor = ImVec4( 0.71f, .51f, .31f, 1.0f );
    ImVec4 theHoverColor = ImVec4( .75f, .48f, .22f, 1.0f );
    ImVec4 theActiveColor = ImVec4( .61f, .36f, .10f, 1.0f );

    ImVec4 theSecondColor = ImVec4( 0.32f, .31f, .29f, 1.0f );
    ImVec4 theSecondHoverColor = ImVec4( .44f, .44f, .40f, 1.0f );
    ImVec4 theSecondActiveColor = ImVec4( .43f, .36f, .29f, 1.0f );

    ImVec4 theThirdColor = ImVec4( 0.18f, .17f, .16f, 1.0f );
    ImVec4 theThirdHoverColor = ImVec4( .40f, .33f, .25f, 1.0f );
    ImVec4 theThirdActiveColor = ImVec4( .43f, .36f, .29f, 1.0f );

    ImVec4* colors = theStyle->Colors;
    colors[ ImGuiCol_DockingEmptyBg ] = ImVec4( 0.0f, 0.0f, 0.0f, 0.0f );

    colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.68f, 0.47f, 0.24f, 1.0f );
    colors[ ImGuiCol_SeparatorHovered ] = ImVec4( .54f, .40f, .08f, 0.75f );
    colors[ ImGuiCol_SeparatorActive ] = ImVec4( .75f, .51f, .1f, 1.0f );

    colors[ ImGuiCol_Button ] = theBaseColor;
    colors[ ImGuiCol_ButtonHovered ] = theHoverColor;
    colors[ ImGuiCol_ButtonActive ] = theActiveColor;
    colors[ ImGuiCol_Header ] = theThirdColor;
    colors[ ImGuiCol_HeaderHovered ] = theThirdHoverColor;
    colors[ ImGuiCol_HeaderActive ] = theThirdActiveColor;
    colors[ ImGuiCol_Tab ] = theBaseColor;
    colors[ ImGuiCol_TabHovered ] = theHoverColor;
    colors[ ImGuiCol_TabActive ] = theActiveColor;
    colors[ ImGuiCol_TitleBgActive ] = theBaseColor;
    colors[ ImGuiCol_ResizeGrip ] = theBaseColor;
    colors[ ImGuiCol_ResizeGripHovered ] = theHoverColor;
    colors[ ImGuiCol_ResizeGripActive ] = theActiveColor;
    colors[ ImGuiCol_SliderGrab ] = theBaseColor;
    colors[ ImGuiCol_SliderGrabActive ] = theActiveColor;
    colors[ ImGuiCol_FrameBg ] = theSecondColor;
    colors[ ImGuiCol_FrameBgHovered ] = theSecondHoverColor;
    colors[ ImGuiCol_FrameBgActive ] = theSecondActiveColor;
    colors[ ImGuiCol_CheckMark ] = theBaseColor;
}


