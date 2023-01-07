//
// Created by Angelo Carly on 27/12/2022.
//

#ifndef VKRT_MESH_H
#define VKRT_MESH_H

#include "vks/core/Buffer.h"
#include "vks/core/Vertex.h"
#include "vks/render/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

namespace vks
{
    class Mesh
    {
        public:
            Mesh();
            Mesh( vks::DevicePtr inDevice, std::vector< vks::Vertex > inVertices, std::vector< uint32_t > inIndices );
            ~Mesh();

        public:
            vks::Buffer GetVertexBuffer();
            vks::Buffer GetIndexBuffer();
            uint32_t GetVertexCount();
            uint32_t GetIndexCount();
        public:
            void Draw( vk::CommandBuffer inCommandBuffer );

        private:
            vks::DevicePtr mDevice;
        private:
            uint32_t mVertexCount;
            uint32_t mIndexCount;
            vks::Buffer mVertexBuffer;
            vks::Buffer mIndexBuffer;
    };
}


#endif //VKRT_MESH_H
