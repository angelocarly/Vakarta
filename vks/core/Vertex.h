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
            Vertex();
            Vertex( glm::vec3 inPosition, glm::vec3 inColor );

        public:
            static std::vector< vk::VertexInputAttributeDescription > GetVkVertexInputAttributeDescriptions();
            static std::vector< vk::VertexInputBindingDescription > GetVkVertexInputBindingDescriptions();

        public:
            glm::vec3 position;
            glm::vec3 color;

    };
}


#endif //VKRT_VERTEX_H
