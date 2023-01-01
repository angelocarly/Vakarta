//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/Vertex.h"

vks::Vertex::Vertex( glm::vec3 inPosition, glm::vec3 inColor )
:
    mPosition( inPosition ),
    mColor( inColor )
{
}

glm::vec3
vks::Vertex::GetPosition()
{
    return mPosition;
}

glm::vec3
vks::Vertex::GetColor()
{
    return mColor;
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
            offsetof( Vertex, mPosition )
        )
    );
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            1,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, mColor )
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
