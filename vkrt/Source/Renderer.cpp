//
// Created by Angelo Carly on 23/11/2022.
//

#include "vkrt/Renderer.h"

vkrt::Renderer::Renderer( vk::SurfaceKHR inSurface )
:
    mInstance( vks::Instance::GetInstance() ),
    mSurface( inSurface ),
    mPhysicalDevice( mInstance ),
    mLogicalDevice( vks::LogicalDevice( mPhysicalDevice ) )
{

}

vkrt::Renderer::~Renderer()
{
    mInstance.GetVulkanInstance().destroy( mSurface );
}

// =====================================================================================================================