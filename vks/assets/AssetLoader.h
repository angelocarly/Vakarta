//
// Created by Angelo Carly on 07/01/2023.
//

#ifndef VKRT_ASSETLOADER_H
#define VKRT_ASSETLOADER_H

#include "MeshResource.h"
#include "ImageResource.h"

#include <filesystem>
#include <memory>

namespace vks
{
    class AssetLoader
    {
        public:
            AssetLoader();
            ~AssetLoader();

            vks::MeshResource LoadMeshResource( std::filesystem::path inPath );
            static vks::ImageResource LoadImageResource( std::filesystem::path inPath );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };

}

#endif //VKRT_ASSETLOADER_H
