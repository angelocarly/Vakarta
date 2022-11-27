//
// Created by Angelo Carly on 23/11/2022.
//

#include "vkrt/Renderer.h"

#include "vks/Instance.h"

vkrt::Renderer::Renderer()
:
    mPhysicalDevice( vks::Instance::GetInstance() ),
    mLogicalDevice( vks::LogicalDevice( mPhysicalDevice ) )
{
}

vkrt::Renderer::~Renderer()
{

}

void vkrt::Renderer::SetupVulkan()
{
//    vks::Instance::GetInstance();
//    instance.GetVulkanInstance();
}
