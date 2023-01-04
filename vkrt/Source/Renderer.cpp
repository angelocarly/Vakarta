//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/Renderer.h"


#include "vks/ForwardDecl.h"
#include "vks/RenderPass.h"
#include "vks/Swapchain.h"
#include "vks/Vertex.h"
#include "vks/Window.h"

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

void
vkrt::Renderer::InitializeRenderObject()
{
    std::vector< vks::Vertex > theVertices =
    {
        vks::Vertex( glm::vec3( 0, 0, 0 ), glm::vec3( 1, 0, 0 ) ),
        vks::Vertex( glm::vec3( 0, 1, 0 ), glm::vec3( 0, 1, 0 ) ),
        vks::Vertex( glm::vec3( 5, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
        vks::Vertex( glm::vec3( 5, 1, 0 ), glm::vec3( 1, 0, 1 ) ),
        vks::Vertex( glm::vec3( 50, 30, 0 ), glm::vec3( 0, 0, 1 ) ),
        vks::Vertex( glm::vec3( 50, 1, 0 ), glm::vec3( 1, 1, 0 ) )
    };
    std::vector< uint32_t > theIndices =
    {
        0, 1, 2,
        1, 2, 3,
        3, 4, 5
    };

//    mMesh = std::make_unique< vks::Mesh >( mDevice, theVertices, theIndices );

    std::istringstream sourceStream( std::filesystem::path( "resources/teapot.obj" ) );
    tinyobj::attrib_t inattrib;
    std::vector<tinyobj::shape_t> inshapes;
    std::vector<tinyobj::material_t> inmaterials;

    std::string warn;
    std::string* err;
    bool ret = tinyobj::LoadObj( &inattrib, &inshapes, &inmaterials, err, "resources/teapot.obj", "resources");
    if (!warn.empty())
    {
        spdlog::warn( "WARNING: {}", warn );
    }
    if (!err->empty())
    {
        spdlog::error( "ERROR: {}", *err );
    }
    if (!ret)
    {
        spdlog::error( "Failed to load file" );
        std::exit( 1 );
    }

    std::vector< vks::Vertex > vertices;
    std::vector< std::uint32_t > indices;
    std::unordered_map< std::uint32_t, std::uint32_t > uniqueVertices;

    for( const auto& shape : inshapes )
    {
        for( const auto& index : shape.mesh.indices )
        {
            glm::vec3 position
            {
                inattrib.vertices[3 * index.vertex_index + 0],
                inattrib.vertices[3 * index.vertex_index + 1],
                inattrib.vertices[3 * index.vertex_index + 2]
            };

            std::uint32_t theHash = abs( ( position.x * 6969 + position.y * 12345 + position.z * 12345566 ) );
            if ( uniqueVertices.count( theHash ) == 0 )
            {
                vertices.push_back( vks::Vertex{ position, glm::vec3(1, 0, 0) } );
                uniqueVertices[ theHash ] = static_cast<uint32_t>(vertices.size());
            }

            indices.push_back( uniqueVertices[ theHash ] );
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
