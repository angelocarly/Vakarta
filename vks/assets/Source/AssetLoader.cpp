//
// Created by Angelo Carly on 07/01/2023.
//

#include "vks/assets/AssetLoader.h"
#include "vks/assets/ImageResource.h"
#include "stb/stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <unordered_map>
#include <spdlog/spdlog.h>

class vks::AssetLoader::Impl
{
    public:
        Impl();
        ~Impl();
};

vks::AssetLoader::Impl::Impl()
{

}

vks::AssetLoader::Impl::~Impl()
{

}

// ---------------------------------------------------------------------------------------------------------------------

vks::AssetLoader::AssetLoader()
:
    mImpl( new Impl() )
{

}

vks::AssetLoader::~AssetLoader()
{

}

struct VecHash
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

vks::MeshResource
vks::AssetLoader::LoadMeshResource( std::filesystem::path inPath )
{
    tinyobj::attrib_t inattrib;
    std::vector<tinyobj::shape_t> inshapes;
    std::vector<tinyobj::material_t> inmaterials;

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj
    (
        &inattrib,
        &inshapes,
        &inmaterials,
        &err,
        inPath.c_str(),
        inPath.root_directory().c_str()
    );
    if (!warn.empty()) spdlog::warn( "WARNING: {}", warn );
    if (!err.empty()) spdlog::error( "ERROR: {}", err );
    if (!ret)
    {
        spdlog::error( "Failed to load file" );
        std::exit( 1 );
    }

    // Map obj file to Resource Mesh
    std::vector< vks::Vertex > vertices;
    std::vector< std::uint32_t > indices;
    std::unordered_map< glm::vec3, std::uint32_t, VecHash, VecHash > uniqueVertices;

    for( const auto& shape : inshapes )
    {
        for( const auto& index : shape.mesh.indices )
        {

            vks::Vertex theVertex;
            theVertex.position.x = inattrib.vertices[3 * index.vertex_index + 0];
            theVertex.position.y = inattrib.vertices[3 * index.vertex_index + 1];
            theVertex.position.z = inattrib.vertices[3 * index.vertex_index + 2];
            theVertex.normal.x = inattrib.normals[3 * index.normal_index + 0];
            theVertex.normal.y = inattrib.normals[3 * index.normal_index + 1];
            theVertex.normal.z = inattrib.normals[3 * index.normal_index + 2];
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

    return { vertices, indices };
}

vks::ImageResource
vks::AssetLoader::LoadImageResource( std::filesystem::path inPath )
{
    std::size_t channelCount = 4;

    int texWidth, texHeight, texChannels;
    stbi_uc * pixels = stbi_load( inPath.c_str(), & texWidth, & texHeight, & texChannels, channelCount );

    if( !pixels )
    {
        throw std::runtime_error( "failed to load texture image!" );
    }

    std::vector< std::uint8_t > thePixels = std::vector< std::uint8_t >( pixels, pixels + texWidth * texHeight * channelCount );
    vks::ImageResource theImage = { ( std::size_t ) texWidth, ( std::size_t ) texHeight, vks::ImageResource::Format::RGBA, std::move( thePixels ) };

    // Free buffer
    stbi_image_free( pixels );

    return theImage;
}
