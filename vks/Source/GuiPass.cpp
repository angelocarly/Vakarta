//
// Created by Angelo Carly on 02/01/2023.
//

#include <spdlog/spdlog.h>
#include "vks/Gui.h"

#include "vks/ForwardDecl.h"
#include "vks/Device.h"
#include "vks/PhysicalDevice.h"
#include "vks/Window.h"

#include "imgui/imgui_impl_vulkan.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui.h"
#include "vks/RenderPass.h"

class vks::Gui::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr mSwapChain );
        ~Impl();

    private:
        void InitializeDescriptorPool();
        void InitializeImGui();
        static void CheckVkResult( VkResult inResult );

    public:
        vks::DevicePtr mDevice;
        vks::WindowPtr mWindow;
        vks::RenderPassPtr mRenderPass;
        vks::SwapchainPtr mSwapChain;

        vk::DescriptorPool mDescriptorPool;
};

void Update();

vks::Gui::Impl::Impl( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr inSwapChain )
:
    mDevice( inDevice ),
    mWindow( inWindow ),
    mRenderPass( inRenderPass ),
    mSwapChain( inSwapChain )
{
    InitializeDescriptorPool();
    InitializeImGui();
}

vks::Gui::Impl::~Impl()
{
    mDevice->GetVkDevice().waitIdle();

    mDevice->GetVkDevice().destroy( mDescriptorPool );

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ::ImGui::DestroyContext();
}

void
vks::Gui::Impl::InitializeDescriptorPool()
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
vks::Gui::Impl::InitializeImGui()
{
    ::IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();
    (void) io;

    io.DisplaySize = ImVec2( mSwapChain->GetExtent().width, mSwapChain->GetExtent().height );
//    io.DisplayFramebufferScale = ImVec2(2.0f, 2.0f);

    ::ImGui::StyleColorsDark();

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
vks::Gui::Impl::CheckVkResult( VkResult inResult )
{
    if ( inResult == 0 ) return;

    spdlog::error( "[ImGui][Vulkan] Error: VkResult = {}", inResult );

    if ( inResult < 0) abort();
}

// ---------------------------------------------------------------------------------------------------------------------

vks::Gui::Gui( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr inSwapChain )
:
    mImpl( new Impl( inDevice, inWindow, inRenderPass, inSwapChain ) )
{

}

vks::Gui::~Gui()
{

}

void
vks::Gui::Render( vk::CommandBuffer inCommandBuffer )
{
    ImGui_ImplVulkan_RenderDrawData( ::ImGui::GetDrawData(), inCommandBuffer );
}

void
vks::Gui::Update()
{
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window;
    ::ImGui::ShowDemoWindow( &show_demo_window );

    // Rendering
    ::ImGui::Render();
}
