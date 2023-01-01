//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/Renderer.h"

#include "vkrt/Window.h"

#include "vks/ForwardDecl.h"
#include "vks/RenderPass.h"
#include "vks/Swapchain.h"
#include "vks/Vertex.h"

#include <spdlog/spdlog.h>

#include <memory>

vkrt::Renderer::Renderer( vkrt::WindowPtr inWindow )
:
    mInstance( vks::Instance::GetInstance() ),
    mWindow( inWindow ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( std::make_shared< vks::Device >( mPhysicalDevice ) ),
    mSwapChain( std::make_unique< vks::Swapchain >( mDevice, mWindow->GetVkSurface() ) ),
    mRenderPass( std::make_unique< vks::RenderPass >( mSwapChain ) ),
    mPipeline( std::make_unique<vks::Pipeline>( mDevice, mRenderPass ) ),
    mCamera( 45, float( mSwapChain->GetExtent().width ) / float( mSwapChain->GetExtent().height ), 0.1f, 100.0f )
{
    InitializeCommandBuffers();
    InitializeRenderObject();

    mCamera.SetPosition( glm::vec3( 0, 0, 20 ) );
}

vkrt::Renderer::~Renderer()
{
    mDevice->GetVkDevice().waitIdle();
}

// =====================================================================================================================

void
vkrt::Renderer::InitializeCommandBuffers()
{
    mCommandBuffers.resize( mSwapChain->GetImageCount() );
    for( int i = 0; i < mCommandBuffers.size(); ++i )
    {
        mCommandBuffers[ i ] = mDevice->GetVkDevice().allocateCommandBuffers
        (
            vk::CommandBufferAllocateInfo( mDevice->GetVkCommandPool(), vk::CommandBufferLevel::ePrimary, 1 )
        ).front();
    }
}

void
vkrt::Renderer::InitializeRenderObject()
{
    std::vector< vks::Vertex > theVertices =
    {
        vks::Vertex( glm::vec3( -1, -1, 0 ) ),
        vks::Vertex( glm::vec3( -1, 1, 0 ) ),
        vks::Vertex( glm::vec3( 1, 1, 0 ) ),
        vks::Vertex( glm::vec3( 1, -1, 0 ) )
    };
    std::vector< uint32_t > theIndices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    mMesh = std::make_unique< vks::Mesh >( mDevice, theVertices, theIndices );
}

// ---------------------------------------------------------------------------------------------------------------------

void vkrt::Renderer::Render()
{
    uint32_t theImageIndex = mSwapChain->RetrieveNextImage();

    auto theCommandBuffer = mCommandBuffers[ theImageIndex ];
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        theCommandBuffer.beginRenderPass( mRenderPass->GetVkBeginInfo( theImageIndex ), vk::SubpassContents::eInline );
        {
            auto theViewport = vk::Viewport
            (
                0,
                0,
                mSwapChain->GetExtent().width,
                mSwapChain->GetExtent().height,
                0.0f,
                1.0f
            );
            theCommandBuffer.setViewport( 0, 1, & theViewport );
            const auto theScissors = vk::Rect2D( { 0, 0 }, mSwapChain->GetExtent() );
            theCommandBuffer.setScissor( 0, 1, & theScissors );

            mPipeline->UpdatePipelineUniforms( mCamera.GetMVP() );
            mPipeline->Bind( theCommandBuffer );
            mMesh->Draw( theCommandBuffer );
        }
        theCommandBuffer.endRenderPass();
    }
    theCommandBuffer.end();

    mSwapChain->SubmitCommandBuffer( theImageIndex, theCommandBuffer );
}
