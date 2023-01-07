//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/Renderer.h"

#include "vks/core/Vertex.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

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
    mPipeline( std::make_unique< vks::Pipeline >( mDevice, mRenderPass ) )
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

struct KeyFuncs
{
    size_t operator()(const glm::vec3& k)const
    {
        return std::hash< float >()( k.x ) ^ std::hash< float >()( k.y ) ^ std::hash< float >()( k.z );
    }

    bool operator()(const glm::vec3& a, const glm::vec3& b)const
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

void
vkrt::Renderer::InitializeRenderObject()
{
    std::istringstream sourceStream( std::filesystem::path( "resources/teapot.obj" ) );
    tinyobj::attrib_t inattrib;
    std::vector<tinyobj::shape_t> inshapes;
    std::vector<tinyobj::material_t> inmaterials;

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj( &inattrib, &inshapes, &inmaterials, &err, "resources/teapot.obj" );
    if (!warn.empty())
    {
        spdlog::warn( "WARNING: {}", warn );
    }
    if (!err.empty())
    {
        spdlog::error( "ERROR: {}", err );
    }
    if (!ret)
    {
        spdlog::error( "Failed to load file" );
        std::exit( 1 );
    }

    std::vector< vks::Vertex > vertices;
    std::vector< std::uint32_t > indices;
    std::unordered_map< glm::vec3, std::uint32_t, KeyFuncs, KeyFuncs > uniqueVertices;

    for( const auto& shape : inshapes )
    {
        for( const auto& index : shape.mesh.indices )
        {

            vks::Vertex theVertex;
            theVertex.position.x = inattrib.vertices[3 * index.vertex_index + 0];
            theVertex.position.y = inattrib.vertices[3 * index.vertex_index + 1];
            theVertex.position.z = inattrib.vertices[3 * index.vertex_index + 2];
            theVertex.color.r = 1.0f;
            theVertex.color.g = 0.0f;
            theVertex.color.b = 0.0f;

            if ( uniqueVertices.count( theVertex.position ) == 0 )
            {
                uniqueVertices[ theVertex.position ] = static_cast<uint32_t>( vertices.size() );
                vertices.push_back( theVertex );
            }

            indices.push_back( uniqueVertices[ theVertex.position ] );
        }
    }
    mMesh = std::make_unique< vks::Mesh >( mDevice, vertices, indices );

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
