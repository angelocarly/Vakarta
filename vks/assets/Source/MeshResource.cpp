//
// Created by Angelo Carly on 07/01/2023.
//

#include "vks/assets/MeshResource.h"

vks::MeshResource::MeshResource()
{

}

vks::MeshResource::MeshResource( std::vector< vks::Vertex > inVertices, std::vector< std::uint32_t > inIndices )
:
    mVertices( inVertices ),
    mIndices( inIndices )
{

}

vks::MeshResource::~MeshResource()
{

}

std::vector< vks::Vertex >
vks::MeshResource::GetVertices()
{
    return mVertices;
}

std::vector< std::uint32_t >
vks::MeshResource::GetIndices()
{
    return mIndices;
}
