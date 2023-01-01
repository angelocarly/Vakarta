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
            Vertex( glm::vec3 inPosition, glm::vec3 inColor );

        public:
            glm::vec3 GetPosition();
            glm::vec3 GetColor();
            static std::vector< vk::VertexInputAttributeDescription > GetVkVertexInputAttributeDescriptions();
            static std::vector< vk::VertexInputBindingDescription > GetVkVertexInputBindingDescriptions();

        private:
            glm::vec3 mPosition;
            glm::vec3 mColor;

    };
}


#endif //VKRT_VERTEX_H
