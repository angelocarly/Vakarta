//
// Created by Angelo Carly on 27/12/2022.
//

#ifndef VKRT_VERTEX_H
#define VKRT_VERTEX_H

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace vks
{
    class Vertex
    {
        public:
            glm::vec3 mPosition;

        public:
            static vk::VertexInputAttributeDescription GetVkVertexInputAttributeDescription();
            static vk::VertexInputBindingDescription GetVkVertexInputBindingDescription();
    };
}


#endif //VKRT_VERTEX_H
