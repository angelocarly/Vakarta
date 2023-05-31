//
// Created by Angelo Carly on 26/11/2022.
//

#ifndef VKS_FORWARDDECL_H
#define VKS_FORWARDDECL_H

#include <memory>

namespace vks
{
    class ComputePipeline;
    typedef std::shared_ptr< vks::ComputePipeline > ComputePipelinePtr;

    class Device;
    typedef std::shared_ptr< vks::Device > DevicePtr;

    class DescriptorSet;
    typedef std::shared_ptr< vks::DescriptorSet > DescriptorSetPtr;

    class GuiPass;
    typedef std::shared_ptr< vks::GuiPass > ImGuiPtr;

    class Pipeline;
    typedef std::shared_ptr< vks::Pipeline > PipelinePtr;

    class PhysicalDevice;
    typedef std::shared_ptr< vks::PhysicalDevice > PhysicalDevicePtr;

    class RenderPass;
    typedef std::shared_ptr< vks::RenderPass > RenderPassPtr;

    class Swapchain;
    typedef std::shared_ptr< vks::Swapchain > SwapchainPtr;

    class VksSession;
    typedef std::shared_ptr< vks::VksSession > VulkanSessionPtr;

    class Window;
    typedef std::shared_ptr< vks::Window > WindowPtr;

}

#endif //VKRT_FORWARDDECL_H
