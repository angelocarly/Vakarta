//
// Created by Angelo Carly on 07/01/2023.
//

#ifndef VKRT_MESHRESOURCE_H
#define VKRT_MESHRESOURCE_H

#include "vks/core/Vertex.h"

#include <vector>

namespace vks
{
    class MeshResource
    {
        public:
            MeshResource();
            MeshResource( std::vector< vks::Vertex > inVertices, std::vector< std::uint32_t > inIndices );
            ~MeshResource();

        public:
            std::vector< vks::Vertex > GetVertices();
            std::vector< std::uint32_t > GetIndices();

        private:
            std::vector< vks::Vertex > mVertices;
            std::vector< std::uint32_t > mIndices;
    };
}

#endif //VKRT_MESHRESOURCE_H
