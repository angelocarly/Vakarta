//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/Renderer.h"

#include "vks/Vertex.h"

#include <spdlog/spdlog.h>

#include <memory>

vkrt::Renderer::Renderer( vk::SurfaceKHR inSurface )
:
    mInstance( vks::Instance::GetInstance() ),
    mSurface( inSurface ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( std::make_shared< vks::Device >( mPhysicalDevice ) ),
    mSwapChain( mDevice, mSurface )
{
    InitializeRenderPass();
    InitializeFrameBuffers();
    InitializeSynchronizationObject();
    InitializeRenderObject();
    mPipeline = std::make_unique<vks::Pipeline>( mDevice, mRenderPass );
}

vkrt::Renderer::~Renderer()
{
    mDevice->GetVkDevice().waitIdle();
    mDevice->GetVkDevice().destroy( mPresentSemaphore );
    mDevice->GetVkDevice().destroy( mRenderPass );

    for( int i = 0; i < mFrameBuffers.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mFrameBuffers[ i ] );
    }
}

// =====================================================================================================================

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
    auto theSwapChainImageViews = mSwapChain.GetSwapChainImageViews();
    mFrameBuffers.resize( theSwapChainImageViews.size() );

    for ( size_t i = 0; i < theSwapChainImageViews.size(); i++)
    {
        auto theSurfaceCapabilities = mPhysicalDevice->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );

        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            1,
            & theSwapChainImageViews[ i ],
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

void vkrt::Renderer::InitializeRenderObject()
{
    std::vector< vks::Vertex > theVertices =
    {
        vks::Vertex( glm::vec3( -1000, -1000, 0 ) ),
        vks::Vertex( glm::vec3( -1000, 1000, 0 ) ),
        vks::Vertex( glm::vec3( 1000, 1000, 0 ) ),
        vks::Vertex( glm::vec3( 1000, -1000, 0 ) )
    };
    std::vector< uint32_t > theIndices =
    {
        0, 1, 2,
        1, 2, 3,
        2, 3, 1
    };

    mMesh = std::make_unique< vks::Mesh >( mDevice, theVertices, theIndices );
}

// ---------------------------------------------------------------------------------------------------------------------

void vkrt::Renderer::Render()
{
    uint32_t theImageIndex = mSwapChain.AcquireNextImage();

    auto theSurfaceCapabilities = mPhysicalDevice->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );

    auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
    {
        auto theClearValue = vk::ClearValue
        (
            vk::ClearColorValue( std::array< float, 4 >( { 0, 0, 0, 0 } ) )
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
            auto theViewport = vk::Viewport( 0, 0, theSurfaceCapabilities.currentExtent.width, theSurfaceCapabilities.currentExtent.height );
            theCommandBuffer.setViewport( 0, 1, & theViewport );

            mPipeline->UpdatePipelineUniforms( theSurfaceCapabilities.currentExtent.width, theSurfaceCapabilities.currentExtent.height );
            mPipeline->Bind( theCommandBuffer );
            mMesh->Draw( theCommandBuffer );
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
