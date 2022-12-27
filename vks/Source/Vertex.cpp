//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/Vertex.h"

vk::VertexInputAttributeDescription
vks::Vertex::GetVkVertexInputAttributeDescription()
{
    return vk::VertexInputAttributeDescription
    (
        0,
        0,
        vk::Format::eR8G8B8A8Unorm,
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
