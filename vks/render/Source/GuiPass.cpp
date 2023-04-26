//
// Created by Angelo Carly on 02/01/2023.
//

#include "vks/render/GuiPass.h"

#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/PhysicalDevice.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Window.h"

#include <spdlog/spdlog.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>
#include <imnodes.h>
#include <vulkan/vulkan.hpp>
#include <filesystem>

class vks::GuiPass::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr mSwapChain );
        ~Impl();

    private:
        void InitializeDescriptorPool();
        void InitializeImGui();
        void SetStyle();
        static void CheckVkResult( VkResult inResult );

    public:
        vks::DevicePtr mDevice;
        vks::WindowPtr mWindow;
        vks::RenderPassPtr mRenderPass;
        vks::SwapchainPtr mSwapChain;

        vk::DescriptorPool mDescriptorPool;

        bool mDemoEnabled = false;
};

void Update();

vks::GuiPass::Impl::Impl( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr inSwapChain )
:
    mDevice( inDevice ),
    mWindow( inWindow ),
    mRenderPass( inRenderPass ),
    mSwapChain( inSwapChain )
{
    InitializeDescriptorPool();
    InitializeImGui();
}

vks::GuiPass::Impl::~Impl()
{
    mDevice->GetVkDevice().waitIdle();

    mDevice->GetVkDevice().destroy( mDescriptorPool );

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImNodes::DestroyContext();
    ::ImGui::DestroyContext();
}

void
vks::GuiPass::Impl::InitializeDescriptorPool()
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
vks::GuiPass::Impl::InitializeImGui()
{
    ::IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();
    SetStyle();

    // Load fonts
    // TODO: Load fonts dynamically
    auto fontPath = std::filesystem::path( "./" ) / "resources" / "fonts" / "Hack-Regular.ttf";
//    io.Fonts->AddFontDefault();
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
    ImGui_ImplVulkan_Init( &init_info, mRenderPass->GetVkRenderPass() );

    // Create fonts
    vk::CommandBuffer theCommandBuffer = mDevice->BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(theCommandBuffer );
    mDevice->EndSingleTimeCommands( theCommandBuffer );
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void
vks::GuiPass::Impl::SetStyle()
{
    ImGuiStyle* theStyle = &ImGui::GetStyle();
    theStyle->WindowBorderSize = 0.0f;
}

void
vks::GuiPass::Impl::CheckVkResult( VkResult inResult )
{
    if ( inResult == 0 ) return;

    spdlog::error( "[ImGui][Vulkan] Error: VkResult = {}", inResult );

    if ( inResult < 0) abort();
}

// ---------------------------------------------------------------------------------------------------------------------

vks::GuiPass::GuiPass( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr inSwapChain )
:
    mImpl( new Impl( inDevice, inWindow, inRenderPass, inSwapChain ) )
{

}

vks::GuiPass::~GuiPass()
{

}

void
vks::GuiPass::Render( vk::CommandBuffer inCommandBuffer )
{
    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), inCommandBuffer );
}

ImGuiContext *
vks::GuiPass::GetImGuiContext()
{
    return ImGui::GetCurrentContext();
}
