//
// Created by Angelo Carly on 03/12/2022.
//

#include "vks/render/Swapchain.h"

#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/PhysicalDevice.h"

#include <spdlog/spdlog.h>

// ---------------------------------------------------------------------------------------------------------------------

class vks::Swapchain::Impl
{
    public:
        Impl( const vks::DevicePtr inDevice, vk::SurfaceKHR inSurface );
        ~Impl();

        vk::Format GetImageFormat() const;
        vk::ColorSpaceKHR GetColorSpace() const;

    public:
        vks::DevicePtr mDevice;
        vk::SurfaceKHR mSurface;
        vk::SwapchainKHR mSwapchain;

    public:
        vk::Extent2D mExtent;
        std::vector< vk::Image > mSwapChainImages;
        std::vector< vk::ImageView > mSwapchainImageViews;

        int mCurrentFrame = 0;
        std::vector< vk::Fence > mCommandBufferExecutedFence;
        std::vector< vk::Semaphore > mImageAcquiredSemaphores;
        std::vector< vk::Semaphore > mBufferExecutedSemaphore;
        std::vector< bool > mFrameWasRendered;

        const int mMinImageCount = 2;

    public:
        void PresentImage( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore );

    private:
        void InitializeSwapChain();
        void InitializeSwapChainImages();
        void InitializeSynchronizationObjects();
};

vks::Swapchain::Impl::Impl( const vks::DevicePtr inDevice, vk::SurfaceKHR inSurface )
:
    mDevice( inDevice ),
    mSurface( inSurface )
{
    InitializeSwapChain();
    InitializeSwapChainImages();
    InitializeSynchronizationObjects();
}

vks::Swapchain::Impl::~Impl()
{
    mDevice->GetVkDevice().waitForFences( mCommandBufferExecutedFence, true, UINT64_MAX );
    mDevice->GetVkDevice().waitIdle();

    for( int i = 0; i < mSwapchainImageViews.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mSwapchainImageViews[ i ] );
    }

    for( auto theFence : mCommandBufferExecutedFence )
    {
        mDevice->GetVkDevice().destroy( theFence );
    }
    for( auto theSemaphore : mImageAcquiredSemaphores )
    {
        mDevice->GetVkDevice().destroy( theSemaphore );
    }
    for( auto theSemaphore : mBufferExecutedSemaphore )
    {
        mDevice->GetVkDevice().destroy( theSemaphore );
    }

    mDevice->GetVkDevice().destroy( mSwapchain );
}

vk::Format
vks::Swapchain::Impl::GetImageFormat() const
{
    return vk::Format::eB8G8R8A8Srgb;
}

vk::ColorSpaceKHR
vks::Swapchain::Impl::GetColorSpace() const
{
    return vk::ColorSpaceKHR::eSrgbNonlinear;
}

void
vks::Swapchain::Impl::InitializeSwapChain()
{
    spdlog::get( "vulkan" )->debug( "Initializing swapchain." );

    // Supported options for the created OS window
    auto physicalDevice = mDevice->GetPhysicalDevice();
    auto surfaceCapabilities = physicalDevice->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );
    auto surfaceFormats = physicalDevice->GetVkPhysicalDevice().getSurfaceFormatsKHR( mSurface );
    auto presentModes = physicalDevice->GetVkPhysicalDevice().getSurfacePresentModesKHR( mSurface );
    mExtent = surfaceCapabilities.currentExtent;

    const uint32_t graphicsFamilyIndex = mDevice->GetPhysicalDevice()->FindQueueFamilyIndices().graphicsFamilyIndex.value();

    auto theFormat = GetImageFormat();
    auto theColorSpace = GetColorSpace();

    // Verify whether the surface supports the colorformat/colorspace
    bool hasFormat = false;
    for( auto surfaceFormat : surfaceFormats )
    {
        if( surfaceFormat.format == theFormat && surfaceFormat.colorSpace == theColorSpace )
        {
            hasFormat = true;
            break;
        }
    }
    if( !hasFormat )
    {
        spdlog::get( "vulkan" )->error
            (
                "The surface doesn't support the requested format ({}) with colorspace ({})",
                to_string( theFormat ),
                to_string( theColorSpace )
            );
        std::exit( 1 );
    }

    vk::SwapchainCreateInfoKHR swapchainCreateInfoKhr
    (
        vk::SwapchainCreateFlagsKHR(),
        mSurface,
        3,
        GetImageFormat(),
        GetColorSpace(),
        mExtent, // Initialize swapchain images with the current extent
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        1,
        &graphicsFamilyIndex,
        vk::SurfaceTransformFlagBitsKHR::eIdentity,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        vk::PresentModeKHR::eFifo,
        true,
        nullptr
    );
    mSwapchain = mDevice->GetVkDevice().createSwapchainKHR( swapchainCreateInfoKhr );

}

void
vks::Swapchain::Impl::InitializeSwapChainImages()
{
    mSwapChainImages = mDevice->GetVkDevice().getSwapchainImagesKHR( mSwapchain );
    mSwapchainImageViews.resize( mSwapChainImages.size() );

    for ( size_t i = 0; i < mSwapchainImageViews.size(); i++)
    {
        auto imageViewCreateInfo = vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mSwapChainImages[ i ],
            vk::ImageViewType::e2D,
            GetImageFormat(),
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

        mSwapchainImageViews[ i ] = mDevice->GetVkDevice().createImageView( imageViewCreateInfo );
    }
}

void
vks::Swapchain::Impl::InitializeSynchronizationObjects()
{
    mFrameWasRendered.resize( mSwapChainImages.size() );

    mCommandBufferExecutedFence.resize( mSwapChainImages.size() );
    for( int i = 0; i < mCommandBufferExecutedFence.size(); ++i )
    {
        auto fenceCreateInfo = vk::FenceCreateInfo
        (
            vk::FenceCreateFlagBits::eSignaled // Mark the first image as signaled
        );
        mCommandBufferExecutedFence[ i ] = mDevice->GetVkDevice().createFence( fenceCreateInfo );
    }

    mImageAcquiredSemaphores.resize( mSwapChainImages.size() );
    for( int i = 0; i < mImageAcquiredSemaphores.size(); ++i )
    {
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
        mImageAcquiredSemaphores[ i ] = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );
    }

    mBufferExecutedSemaphore.resize( mSwapChainImages.size() );
    for( int i = 0; i < mBufferExecutedSemaphore.size(); ++i )
    {
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() );
        mBufferExecutedSemaphore[ i ] = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

vks::Swapchain::Swapchain( const vks::DevicePtr inLogicalDevice, vk::SurfaceKHR inSurface )
    :
    mImpl( new Impl( inLogicalDevice, inSurface ) )
{
}

vks::Swapchain::~Swapchain()
{
}

vk::Format vks::Swapchain::GetImageFormat()
{
    return mImpl->GetImageFormat();
}

/**
 * Wait for the next presentable image index.
 * @return
 */
int
vks::Swapchain::RetrieveNextImage()
{
    while( vk::Result::eTimeout == mImpl->mDevice->GetVkDevice().waitForFences( mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ], VK_TRUE, UINT64_MAX ));
    mImpl->mDevice->GetVkDevice().resetFences( mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ] );

    // Block until a new image is acquired
    uint32_t imageIndex = 0;
    mImpl->mDevice->GetVkDevice().acquireNextImageKHR
    (
        mImpl->mSwapchain,
        UINT64_MAX,
        // The semaphore to signal when the image is acquired
        mImpl->mImageAcquiredSemaphores[ mImpl->mCurrentFrame ],
        nullptr,
        & imageIndex
    );

    // Reset the fence
    mImpl->mDevice->GetVkDevice().resetFences( mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ] );

    return imageIndex;
}

/**
 * Submit a command buffer for image @inImageIndex
 * @param inImageIndex
 * @param inCommandBuffer
 */
void
vks::Swapchain::SubmitCommandBuffer( std::uint32_t inImageIndex, vk::CommandBuffer inCommandBuffer )
{

    vk::PipelineStageFlags theDstStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    mImpl->mDevice->GetVkQueue().submit
    (
        vk::SubmitInfo
        (
            1,
            // Wait for the swapchain image to be acquired
            & mImpl->mImageAcquiredSemaphores[ mImpl->mCurrentFrame ],
            & theDstStageFlags,
            1,
            & inCommandBuffer,
            1,
            // Signal the semaphore when the command buffer has finished executing
            &mImpl->mBufferExecutedSemaphore[ mImpl->mCurrentFrame ]
        ),
        // Fence to trigger when the command buffer has finished executing
        mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ]
    );

    // Display the swapchain image
    mImpl->PresentImage( inImageIndex, mImpl->mBufferExecutedSemaphore[ mImpl->mCurrentFrame ] );

    mImpl->mCurrentFrame = ( mImpl->mCurrentFrame + 1 ) % mImpl->mSwapChainImages.size();
}

/**
 * Present a swapchain image
 * NOTE: During debugging, it is possible that a frame is seemingly skipped and the same image is rendered twice.
 * It is unclear why this happens, when taking about five seconds between frames, the issue does not occur.
 * @param inImageIndex the image to render
 * @param inWaitSemaphore semaphore to trigger before submission
 */
void
vks::Swapchain::Impl::PresentImage( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore )
{
    // Submit an image for presentation
    auto thePresentInfo = vk::PresentInfoKHR
    (
        inWaitSemaphore,
        mSwapchain,
        inImageIndex
    );

    auto result = mDevice->GetVkQueue().presentKHR( thePresentInfo );
    if( result != vk::Result::eSuccess )
    {
        // TODO: recreate the swapchain
        throw std::runtime_error( "Error presenting swapchain" );
    }
}

vks::DevicePtr
vks::Swapchain::GetDevice()
{
    return mImpl->mDevice;
}

std::vector< vk::ImageView >
vks::Swapchain::GetSwapchainImageViews() const
{
    return mImpl->mSwapchainImageViews;
}

vk::Extent2D
vks::Swapchain::GetExtent() const
{
    return mImpl->mExtent;
}

int
vks::Swapchain::GetImageCount() const
{
    return mImpl->mSwapChainImages.size();
}

int
vks::Swapchain::GetMinImageCount() const
{
    return mImpl->mMinImageCount;
}
