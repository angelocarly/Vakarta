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

    auto theImage = vks::AssetLoader::LoadImageResource( "resources/image.png" );

    mImage = std::make_shared< vks::Image >( mDevice->CreateImage
    (
        vk::Format::eR8G8B8A8Unorm,
        theImage.GetWidth(),
        theImage.GetHeight(),
        vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
        vma::MemoryUsage::eGpuOnly
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

        // Load image

        // Staging buffer
        vma::AllocationCreateInfo theVertexAllocationInfo;
        theVertexAllocationInfo.usage = vma::MemoryUsage::eAuto;
        theVertexAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
        theStagingBuffer = mDevice->CreateBuffer
        (
            vk::BufferCreateInfo
            (
                vk::BufferCreateFlags(),
                theImage.GetDataSize(),
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc
            ),
            theVertexAllocationInfo
        );

        // Copy image to staging buffer
        void* theMemory = mDevice->MapMemory( theStagingBuffer );
        std::memcpy(theMemory, theImage.GetData().data(), theImage.GetDataSize() );
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
                vk::Extent3D( theImage.GetWidth(), theImage.GetHeight(), 1 )
            )
        );

        // Transfer image layout to shader read
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            * mImage,
            vk::AccessFlagBits::eTransferWrite,
            vk::AccessFlagBits::eShaderRead,
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::DependencyFlagBits::eByRegion
        );
    }
    mDevice->EndSingleTimeCommands( theCommandBuffer );

    mDevice->DestroyBuffer( theStagingBuffer );
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
