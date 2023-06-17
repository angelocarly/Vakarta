/**
 * Presenter.h
 *
 * @file	Presenter.h
 * @author	Angelo Carly
 * @date	11/06/2023
 *
 */

#ifndef VKRT_PRESENTER_H
#define VKRT_PRESENTER_H

#include "RenderEnvironment.h"

#include <cstddef>
#include <cstdint>

namespace vkrt
{
    class Presenter
    {
        public:

            Presenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight );
            virtual ~Presenter();

            virtual void Draw( vkrt::RenderEnvironment const & inRenderEnvironment ) = 0;
            vk::ImageView GetImageView() const { return mImageView; };
            vk::Sampler GetSampler() const { return mSampler; };

        protected:
            vk::RenderPass GetRenderPass() const { return mRenderPass; };
            void BeginRenderPass( vk::CommandBuffer const & inCommandBuffer );
            void EndRenderPass( vk::CommandBuffer const & inCommandBuffer );

        private:
            void InitializeImage();
            void InitializeRenderPass();
            void InitializeFrameBuffer( vk::ImageView inImageView, std::size_t inWidth, std::size_t inHeight );

        private:
            vks::DevicePtr mDevice;

            // Render setup
            vk::RenderPass mRenderPass;
            vk::Framebuffer mFrameBuffer;

            // Render target
            vk::ClearValue mClearValue = vk::ClearValue( vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 1.0f } ) );
            vk::Format mImageFormat = vk::Format::eR8G8B8A8Unorm;
            std::uint32_t mWidth;
            std::uint32_t mHeight;
            vks::Image mImage;
            vk::ImageView mImageView;
            vk::Sampler mSampler;
    };
}

#endif
