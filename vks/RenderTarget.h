
#ifndef PACKZ_RENDERTARGET_H
#define PACKZ_RENDERTARGET_H

#include "vks/ForwardDecl.h"
#include "vks/Image.h"
#include "vks/Buffer.h"

#include <vulkan/vulkan.hpp>

#include <memory>

namespace vks
{
    class RenderTarget
    {
        public:
            RenderTarget( vks::DevicePtr inDevice, vk::RenderPass inRenderPass, size_t inWidth, size_t inHeight );
            ~RenderTarget();

            size_t GetWidth();
            size_t GetHeight();

            vk::Framebuffer GetFrameBuffer();
            vks::Image GetFrameBufferImage();
            vk::ImageView GetFrameBufferImageView();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //PACKZ_RENDERTARGET_H
