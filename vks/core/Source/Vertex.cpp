//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/core/Vertex.h"

vks::Vertex::Vertex()
:
    position( glm::vec3() ),
    color( glm::vec3() )
{
}

vks::Vertex::Vertex( glm::vec3 inPosition, glm::vec3 inColor )
:
    position( inPosition ),
    color( inColor )
{
}

std::vector< vk::VertexInputAttributeDescription >
vks::Vertex::GetVkVertexInputAttributeDescriptions()
{
    std::vector< vk::VertexInputAttributeDescription > theAttributes;
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            0,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, position )
        )
    );
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            1,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, normal )
        )
    );
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            2,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, color )
        )
    );
    return theAttributes;
}

std::vector< vk::VertexInputBindingDescription >
vks::Vertex::GetVkVertexInputBindingDescriptions()
{
    std::vector< vk::VertexInputBindingDescription > theDescriptions;
    theDescriptions.push_back
    (
        vk::VertexInputBindingDescription
        (
            0,
            sizeof( Vertex ),
            vk::VertexInputRate::eVertex
        )
    );
    return theDescriptions;
}

