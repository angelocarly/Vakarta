//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/render/Utils.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Device.h"

#include <vulkan/vulkan.hpp>

#include <filesystem>
#include <fstream>
#include <vector>

namespace
{
    std::vector< char > const
    ReadFile( std::filesystem::path const & inPath )
    {
        std::error_code theError;

        if( !std::filesystem::exists( inPath ) )
        {
            std::string theError = "Could not find shader: " + inPath.string();
            throw std::runtime_error( theError );
        }

        std::vector< char > theBuffer( std::filesystem::file_size( inPath, theError ) );

        std::ifstream theFile( inPath );
        theFile.read( theBuffer.data(), theBuffer.size() );

        return theBuffer;
    }
}

vk::ShaderModule
vks::Utils::CreateVkShaderModule( vks::DevicePtr inDevice, std::filesystem::path inPath )
{
    auto const theShaderCode = ReadFile( inPath );

    vk::ShaderModuleCreateInfo theShaderModuleCreateInfo
        (
            vk::ShaderModuleCreateFlags(),
            theShaderCode.size(),
            reinterpret_cast< uint32_t const * >( theShaderCode.data() )
        );

    return inDevice->GetVkDevice().createShaderModule( theShaderModuleCreateInfo );
}

bool vks::Utils::IsTargetApple()
{
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
}
