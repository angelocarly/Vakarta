//
// Created by Angelo Carly on 23/11/2022.
//

#include <spdlog/spdlog.h>

#include <memory>
#include "vkrt/Renderer.h"

vkrt::Renderer::Renderer( vk::SurfaceKHR inSurface )
:
    mInstance( vks::Instance::GetInstance() ),
    mSurface( inSurface ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( std::make_shared< vks::Device >( mPhysicalDevice ) ),
    mSwapChain( mDevice, mSurface )
{
    CreateSwapChainImageViews();
    InitializeRenderPass();
    InitializeFrameBuffers();
    InitializeSynchronizationObject();
    mPipeline = std::make_unique<vks::Pipeline>( mDevice, mRenderPass );
}

vkrt::Renderer::~Renderer()
{
    mDevice->GetVkDevice().waitIdle();

    mDevice->GetVkDevice().destroy( mPresentSemaphore );

    mDevice->GetVkDevice().destroy( mRenderPass );

    for( int i = 0; i < mSwapChainImageViews.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mSwapChainImageViews[ i ] );
        mDevice->GetVkDevice().destroy( mFrameBuffers[ i ] );
    }
}

// =====================================================================================================================

void
vkrt::Renderer::CreateSwapChainImageViews()
{
    auto swapChainImages = mSwapChain.GetSwapChainImages();
    mSwapChainImageViews.resize( swapChainImages.size() );

    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        auto imageViewCreateInfo = vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            swapChainImages[ i ],
            vk::ImageViewType::e2D,
            vk::Format::eB8G8R8A8Unorm,
            vk::ComponentMapping
            (
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity
            ),
            vk::ImageSubresourceRange
            (
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        );

        mSwapChainImageViews[ i ] = mDevice->GetVkDevice().createImageView( imageViewCreateInfo );
    }
}

void
vkrt::Renderer::InitializeRenderPass()
{
    auto theColorAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        mSwapChain.GetImageFormat(),
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    );

    auto theColorAttachmentRef = vk::AttachmentReference
    (
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &theColorAttachmentRef
    );

    auto theRenderPassCreateInfo = vk::RenderPassCreateInfo
    (
        vk::RenderPassCreateFlags(),
        1,
        &theColorAttachmentDescription,
        1,
        &theSubpassDescription,
        0,
        nullptr
    );

    mRenderPass = mDevice->GetVkDevice().createRenderPass( theRenderPassCreateInfo );

}

void
vkrt::Renderer::InitializeFrameBuffers()
{
    auto swapChainImages = mSwapChain.GetSwapChainImages();
    mFrameBuffers.resize( swapChainImages.size() );

    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        auto theSurfaceCapabilities = mPhysicalDevice->GetVulkanPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );

        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            1,
            & mSwapChainImageViews[i],
            theSurfaceCapabilities.currentExtent.width,
            theSurfaceCapabilities.currentExtent.height,
            1
        );
        mFrameBuffers[i] = mDevice->GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
    }
}

void vkrt::Renderer::InitializeSynchronizationObject()
{
    mPresentSemaphore = mDevice->GetVkDevice().createSemaphore
    (
        vk::SemaphoreCreateInfo
        (
            vk::SemaphoreCreateFlags()
        )
    );

}

// ---------------------------------------------------------------------------------------------------------------------

void vkrt::Renderer::Render()
{
    uint32_t theImageIndex = mSwapChain.AcquireNextImage();

    auto theSurfaceCapabilities = mPhysicalDevice->GetVulkanPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );

    auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
    {
        auto theClearValue = vk::ClearValue
        (
            vk::ClearColorValue( std::array< float, 4 >( { 1, 0, 0, 0 } ) )
        );
        auto theRenderPassBeginInfo = vk::RenderPassBeginInfo
        (
            mRenderPass,
            mFrameBuffers[ theImageIndex ],
            vk::Rect2D( vk::Offset2D(), theSurfaceCapabilities.currentExtent ),
            1,
            & theClearValue
        );
        theCommandBuffer.beginRenderPass( theRenderPassBeginInfo, vk::SubpassContents::eInline );
        {

            mPipeline->Bind( theCommandBuffer );

        }
        theCommandBuffer.endRenderPass();
    }
    theCommandBuffer.end();

    // Submit command buffer and wait until completion
    vk::Fence theFence = mDevice->GetVkDevice().createFence( vk::FenceCreateInfo() );
    mDevice->GetVkQueue().submit
    ( vk::SubmitInfo
        (
            0,
            nullptr,
            nullptr,
            1,
            &theCommandBuffer,
            1,
            & mPresentSemaphore
        ),
        theFence
    );
    while( vk::Result::eTimeout == mDevice->GetVkDevice().waitForFences( theFence, VK_TRUE, UINT64_MAX ));
    mDevice->GetVkDevice().destroyFence( theFence );
    mDevice->GetVkDevice().freeCommandBuffers( mDevice->GetVkCommandPool(), theCommandBuffer );

    // Display the swapchain image
    mSwapChain.PresentSwapChain( theImageIndex, mPresentSemaphore );
}
