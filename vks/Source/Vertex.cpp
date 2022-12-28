//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/Vertex.h"

vks::Vertex::Vertex( glm::vec3 inPosition )
:
    mPosition( inPosition )
{

}

glm::vec3 vks::Vertex::GetPosition()
{
    return mPosition;
}

vk::VertexInputAttributeDescription
vks::Vertex::GetVkVertexInputAttributeDescription()
{
    return vk::VertexInputAttributeDescription
    (
        0,
        0,
        vk::Format::eR32G32B32Sfloat,
        0
    );
}

vk::VertexInputBindingDescription
vks::Vertex::GetVkVertexInputBindingDescription()
{
    return vk::VertexInputBindingDescription
    (
        0,
        sizeof( mPosition ),
        vk::VertexInputRate::eVertex
    );
}
