//
// Created by Angelo Carly on 27/12/2022.
//

#ifndef VKRT_MESH_H
#define VKRT_MESH_H

#include <vulkan/vulkan.hpp>
#include "Buffer.h"
#include "Vertex.h"
#include "ForwardDecl.h"

namespace vks
{
    class Mesh
    {
        public:
            Mesh();
            Mesh( vks::DevicePtr inDevice, std::vector< vks::Vertex > inVertices, std::vector< uint32_t > inIndices );
            ~Mesh();

        public:
            void Draw( vk::CommandBuffer inCommandBuffer );

            vks::Buffer GetVertexBuffer();
            vks::Buffer GetIndexBuffer();
            uint32_t GetVertexCount();
            uint32_t GetIndexCount();

        private:
            vks::DevicePtr mDevice;

            uint32_t mVertexCount;
            uint32_t mIndexCount;
            vks::Buffer mVertexBuffer;
            vks::Buffer mIndexBuffer;
    };
}


#endif //VKRT_MESH_H
