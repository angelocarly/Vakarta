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
    CreateSwapChainImageViews();
    InitializeRenderPass();
}

vkrt::Renderer::~Renderer()
{

    mLogicalDevice->GetVulkanDevice().destroy( mRenderPass );

    for( int i = 0; i < mSwapChainImageViews.size(); i++ )
    {
        mLogicalDevice->GetVulkanDevice().destroy( mSwapChainImageViews[ i ] );
    }
}

// =====================================================================================================================

void
vkrt::Renderer::CreateSwapChainImageViews()
{
    auto swapChainImages = mSwapChain.GetSwapChainImages();
    mSwapChainImageViews.resize( swapChainImages.size() );

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        auto imageViewCreateInfo = vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            swapChainImages[ i ],
            vk::ImageViewType::e2D,
            vk::Format::eB8G8R8A8Unorm,
            vk::ComponentMapping
            (
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity
            ),
            vk::ImageSubresourceRange
            (
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        );

        mSwapChainImageViews[ i ] = mLogicalDevice->GetVulkanDevice().createImageView( imageViewCreateInfo );
    }
}

void
vkrt::Renderer::InitializeRenderPass()
{
    auto theColorAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        mSwapChain.GetImageFormat(),
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal
    );

    auto theColorAttachmentRef = vk::AttachmentReference
    (
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &theColorAttachmentRef
    );

    auto theRenderPassCreateInfo = vk::RenderPassCreateInfo
    (
        vk::RenderPassCreateFlags(),
        1,
        &theColorAttachmentDescription,
        1,
        &theSubpassDescription,
        0,
        nullptr
    );

    mRenderPass = mLogicalDevice->GetVulkanDevice().createRenderPass( theRenderPassCreateInfo );

}

// ---------------------------------------------------------------------------------------------------------------------

void
vkrt::Renderer::Render()
{
    mSwapChain.AcquireNextImage();
}