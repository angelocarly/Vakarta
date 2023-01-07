//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/Renderer.h"

#include "vks/core/Vertex.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <memory>
#include <unordered_map>

vkrt::Renderer::Renderer( vks::WindowPtr inWindow )
:
    mInstance( vks::Instance::GetInstance() ),
    mWindow( inWindow ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( std::make_shared< vks::Device >( mPhysicalDevice ) ),
    mSwapChain( std::make_shared< vks::Swapchain >( mDevice, mWindow->GetVkSurface() ) ),
    mRenderPass( std::make_shared< vks::RenderPass >( mSwapChain ) ),
    mPipeline( std::make_unique< vks::Pipeline >( mDevice, mRenderPass ) ),
    mAssetLoader()
{
InitializeCommandBuffers();
    InitializeRenderObject();
    mGui = std::make_shared<vks::GuiPass>( mDevice, mWindow, mRenderPass, mSwapChain );
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

    auto theMesh = mAssetLoader.LoadMeshResource( "resources/bunny.obj" );
    mMesh = std::make_unique< vks::Mesh >( mDevice, theMesh.GetVertices(), theMesh.GetIndices() );

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

            mPipeline->UpdatePipelineUniforms( mCamera->GetMVP() );
            mPipeline->Bind( theCommandBuffer );
            mMesh->Draw( theCommandBuffer );

            mGui->Update();
            mGui->Render( theCommandBuffer );
        }
        theCommandBuffer.endRenderPass();
    }
    theCommandBuffer.end();

    mSwapChain->SubmitCommandBuffer( theImageIndex, theCommandBuffer );
}

void
vkrt::Renderer::SetCamera( vkrt::CameraPtr inCamera )
{
    mCamera = inCamera;
}
