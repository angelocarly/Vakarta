//
// Created by Angelo Carly on 27/12/2022.
//

#ifndef VKRT_UTILS_H
#define VKRT_UTILS_H

#include "ForwardDecl.h"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <filesystem>

namespace vks
{
    class Utils
    {
        public:
            static vk::ShaderModule CreateVkShaderModule( vks::DevicePtr inDevice, std::filesystem::path inPath );
            static bool IsTargetApple();
    };
}


#endif //VKRT_UTILS_H
