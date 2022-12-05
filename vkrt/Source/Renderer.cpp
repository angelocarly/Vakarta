//
// Created by Angelo Carly on 23/11/2022.
//

#include <spdlog/spdlog.h>
#include "vkrt/Renderer.h"

vkrt::Renderer::Renderer( vk::SurfaceKHR inSurface )
:
    mInstance( vks::Instance::GetInstance() ),
    mSurface( inSurface ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mLogicalDevice( std::make_shared< vks::LogicalDevice >( mPhysicalDevice ) ),
    mSwapChain( mLogicalDevice, mSurface )
{
}

vkrt::Renderer::~Renderer()
{
}

// =====================================================================================================================