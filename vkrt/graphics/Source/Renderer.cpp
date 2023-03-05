//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/graphics/Renderer.h"

#include "vkrt/graphics/MeshPipeline.h"

#include "vks/core/Vertex.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"
#include "vkrt/graphics/GeoGenPipeline.h"
#include "vkrt/graphics/LinePipeline.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <memory>
#include <unordered_map>

vkrt::Renderer::Renderer( vks::VulkanSessionPtr inSession, vks::WindowPtr inWindow, vkrt::RendererConfig inConfig )
:
    mWindow( inWindow ),
    mDevice( inSession->GetDevice() ),
    mConfig( inConfig ),
    mSwapChain( std::make_shared< vks::Swapchain >( mDevice, mWindow->GetVkSurface() ) ),
    mRenderPass( std::make_shared< vks::RenderPass >( mSwapChain ) ),
    mMeshPipeline( std::make_unique< vkrt::MeshPipeline >( mDevice, mRenderPass, vk::PrimitiveTopology::eTriangleList ) ),
    mLinePipeline( std::make_unique< vkrt::LinePipeline >( mDevice, mRenderPass ) ),
    mGeoGenPipeline( std::make_unique< vkrt::GeoGenPipeline >( mDevice, mRenderPass ) )
{
    InitializeCommandBuffers();
    mGui = std::make_shared<vks::GuiPass>( mDevice, mWindow, mRenderPass, mSwapChain );
    InitializeBuffers();
}

vkrt::Renderer::~Renderer()
{
    mDevice->DestroyBuffer( mVertexBuffer );
    mDevice->DestroyBuffer( mIndexBuffer );
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
vkrt::Renderer::InitializeBuffers()
{
    vma::AllocationCreateInfo theUniformBufferAllocationInfo;
    theUniformBufferAllocationInfo.usage = vma::MemoryUsage::eAuto;
    theUniformBufferAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;

    mVertexBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo
        (
            vk::BufferCreateFlags(),
            mInstanceCount * sizeof( glm::vec4 ) * mVerticesPerInstance,
            vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer
        ),
        theUniformBufferAllocationInfo
    );

    vma::AllocationCreateInfo theIndexUniformBufferAllocationInfo;
    theIndexUniformBufferAllocationInfo.usage = vma::MemoryUsage::eAuto;
    theIndexUniformBufferAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;

    mIndexBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo
        (
            vk::BufferCreateFlags(),
            mInstanceCount * sizeof( std::uint32_t ) * mIndicesPerInstance,
            vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eIndexBuffer
        ),
        theIndexUniformBufferAllocationInfo
    );
    spdlog::info( "Created vertex buffer of size: {}", mVertexBuffer.GetSize() );
    spdlog::info( "Created index buffer of size: {}", mIndexBuffer.GetSize() );

    mGeoGenPipeline->UpdateDescriptorSet( mVertexBuffer, mIndexBuffer );

    auto compBuffer = mDevice->BeginSingleTimeCommands();
    mGeoGenPipeline->Bind( compBuffer );
    mGeoGenPipeline->Dispatch( compBuffer, mInstanceCount, true );
    mDevice->EndSingleTimeCommands( compBuffer );
}

// ---------------------------------------------------------------------------------------------------------------------

void
vkrt::Renderer::RenderFrame( vks::Mesh & inMesh )
{
    uint32_t theImageIndex = mSwapChain->RetrieveNextImage();
    mDevice->GetVkDevice().waitIdle();

    auto theCommandBuffer = mCommandBuffers[ theImageIndex ];
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {

        {
            auto theIndexBufferMemoryBarrier = vk::BufferMemoryBarrier
            (
                vk::AccessFlagBits::eShaderWrite,
                vk::AccessFlagBits::eMemoryRead,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                mIndexBuffer.GetVkBuffer(),
                0,
                mIndexBuffer.GetSize()
            );

            theCommandBuffer.pipelineBarrier
            (
                vk::PipelineStageFlagBits::eComputeShader,
                vk::PipelineStageFlagBits::eVertexInput,
                {},
                nullptr,
                theIndexBufferMemoryBarrier,
                nullptr
            );

            auto theVertexBufferMemoryBarrier = vk::BufferMemoryBarrier
            (
                vk::AccessFlagBits::eShaderWrite,
                vk::AccessFlagBits::eMemoryRead,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                mVertexBuffer.GetVkBuffer(),
                0,
                mVertexBuffer.GetSize()
            );

            theCommandBuffer.pipelineBarrier
            (
                vk::PipelineStageFlagBits::eComputeShader,
                vk::PipelineStageFlagBits::eVertexInput,
                {},
                nullptr,
                theVertexBufferMemoryBarrier,
                nullptr
            );
        }

        theCommandBuffer.beginRenderPass( mRenderPass->GetVkBeginInfo( theImageIndex ), vk::SubpassContents::eInline );
        {
            // Set viewport and scissor
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

            // Draw commands
            switch( mConfig.topology )
            {
                case RendererConfig::TRIANGLES:
                    mMeshPipeline->UpdatePipelineUniforms( mCamera->GetMVP() );
                    mMeshPipeline->Bind( theCommandBuffer );
                    break;
                case RendererConfig::LINES:
                    mLinePipeline->UpdatePipelineUniforms( mCamera->GetMVP() );
                    mLinePipeline->Bind( theCommandBuffer );
                    break;
            }
//            inMesh.Draw( theCommandBuffer );
            theCommandBuffer.bindVertexBuffers( 0, { mVertexBuffer.GetVkBuffer() }, { 0 } );
            theCommandBuffer.bindIndexBuffer( mIndexBuffer.GetVkBuffer(), 0, vk::IndexType::eUint32 );
            theCommandBuffer.drawIndexed( mInstanceCount * mIndicesPerInstance, 1, 0, 0, 0 );

            // Gui
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

ImGuiContext *
vkrt::Renderer::GetImGuiContext()
{
    return mGui->GetImGuiContext();
}

void
vkrt::Renderer::SetConfig( vkrt::RendererConfig inConfig )
{
    mConfig = inConfig;
}

vkrt::RendererConfig
vkrt::Renderer::GetConfig()
{
    return mConfig;
}

void
vkrt::Renderer::Recompute()
{
    auto compBuffer = mDevice->BeginSingleTimeCommands();
    mGeoGenPipeline->Bind( compBuffer );
    mGeoGenPipeline->Dispatch( compBuffer, mInstanceCount, false );
    mDevice->EndSingleTimeCommands( compBuffer );
}
