/**
 * ReflectionPresenter.cpp
 *
 * @file	ReflectionPresenter.cpp
 * @author	Angelo Carly
 * @date	10/04/2023
 *
 */

#include "vkrt/graphics/ReflectionPresenter.h"

#include "vks/render/Device.h"
#include "vks/render/VulkanSession.h"
#include "imgui_impl_vulkan.h"

// TODO: put in loader class
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

vkrt::ReflectionPresenter::ReflectionPresenter( vks::VulkanSessionPtr inSession )
:
    mDevice( inSession->GetDevice() )
{
    mImage = std::make_shared< vks::Image >( mDevice->CreateImage
    (
        vk::ImageCreateInfo
        (
            vk::ImageCreateFlags(),
            vk::ImageType::e2D,
            vk::Format::eR8G8B8A8Unorm,
            vk::Extent3D( 2048, 2048, 1 ),
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
            vk::SharingMode::eExclusive,
            1,
            0,
            vk::ImageLayout::eUndefined
        ),
        vma::AllocationCreateInfo
        (
            vma::AllocationCreateFlagBits::eDedicatedMemory,
            vma::MemoryUsage::eAuto
        )
    ) );

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

    vks::Buffer theStagingBuffer;
    auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
    {
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            * mImage,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eTransferRead,
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eTransfer,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal,
            vk::DependencyFlagBits::eByRegion
        );

        int texWidth, texHeight, texChannels;
        stbi_uc * pixels = stbi_load( "resources/image.png", & texWidth, & texHeight, & texChannels, STBI_rgb_alpha );
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if( !pixels )
        {
            throw std::runtime_error( "failed to load texture image!" );
        }

        // Staging buffer
        vma::AllocationCreateInfo theVertexAllocationInfo;
        theVertexAllocationInfo.usage = vma::MemoryUsage::eAuto;
        theVertexAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        theStagingBuffer = mDevice->CreateBuffer
        (
            vk::BufferCreateInfo
            (
                vk::BufferCreateFlags(),
                imageSize,
                vk::BufferUsageFlagBits::eStorageBuffer
            ),
            theVertexAllocationInfo
        );

        // Copy image to staging buffer
        void* theMemory = mDevice->MapMemory( theStagingBuffer );
        memcpy(theMemory, pixels, static_cast<size_t>( imageSize ) );
        mDevice->UnmapMemory( theStagingBuffer );

        // Copy staging buffer to image memory
        theCommandBuffer.copyBufferToImage
        (
            theStagingBuffer.GetVkBuffer(),
            mImage->GetVkImage(),
            vk::ImageLayout::eTransferDstOptimal,
            vk::BufferImageCopy
            (
                0,
                0,
                0,
                vk::ImageSubresourceLayers
                (
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    0,
                    1
                ),
                vk::Offset3D( 0, 0, 0 ),
                vk::Extent3D( texWidth, texHeight, 1 )
            )
        );

        // Transfer image layout to shader read
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            * mImage,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eShaderRead,
            vk::PipelineStageFlagBits::eNone,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::DependencyFlagBits::eByRegion
        );

        // Free buffer
        stbi_image_free( pixels );
    }
    mDevice->EndSingleTimeCommands( theCommandBuffer );

    mDevice->DestroyBuffer( theStagingBuffer );
}

vkrt::ReflectionPresenter::~ReflectionPresenter()
{
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
