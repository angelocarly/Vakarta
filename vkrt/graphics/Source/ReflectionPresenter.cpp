/**
 * ReflectionPresenter.cpp
 *
 * @file	ReflectionPresenter.cpp
 * @author	Angelo Carly
 * @date	10/04/2023
 *
 */

#include "vkrt/graphics/ReflectionPresenter.h"

#include "vks/assets/AssetLoader.h"
#include "vks/render/Device.h"
#include "vks/render/VulkanSession.h"
#include "imgui_impl_vulkan.h"

vkrt::ReflectionPresenter::ReflectionPresenter( vks::VulkanSessionPtr inSession )
:
    mDevice( inSession->GetDevice() )
{

    auto theImageResource = vks::AssetLoader::LoadImageResource( "resources/image.png" );

    mImage = std::make_shared< vks::Image >( mDevice->AllocateImage( theImageResource, vk::ImageLayout::eShaderReadOnlyOptimal ) );

    auto imageViewCreateInfo = vk::ImageViewCreateInfo
    (
        vk::ImageViewCreateFlags(),
        mImage->GetVkImage(),
        vk::ImageViewType::e2D,
        vk::Format::eR8G8B8A8Unorm,
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
    mImageView = mDevice->GetVkDevice().createImageView( imageViewCreateInfo );

    mSampler = mDevice->GetVkDevice().createSampler
    (
        vk::SamplerCreateInfo
        (
            vk::SamplerCreateFlags(),
            vk::Filter::eLinear,
            vk::Filter::eLinear,
            vk::SamplerMipmapMode::eLinear,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            VK_FALSE,
            16.0f,
            VK_FALSE,
            vk::CompareOp::eAlways,
            0.0f,
            0.0f,
            vk::BorderColor::eIntOpaqueBlack,
            VK_FALSE
        )
    );
}

vkrt::ReflectionPresenter::~ReflectionPresenter()
{
    mDevice->GetVkDevice().destroySampler( mSampler );
    mDevice->GetVkDevice().destroyImageView( mImageView );
    mDevice->DestroyImage( *mImage );
}

vk::DescriptorSet
vkrt::ReflectionPresenter::GetImage()
{
    if( !mImageDescriptorSet )
    {
        mImageDescriptorSet = ImGui_ImplVulkan_AddTexture( mSampler, mImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
    }
    return mImageDescriptorSet;
}
