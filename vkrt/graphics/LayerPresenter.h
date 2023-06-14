/**
 * LayerPresenter.h
 *
 * @file	LayerPresenter.h
 * @author	Angelo Carly
 * @date	13/06/2023
 *
 */

#ifndef VKRT_LAYERPRESENTER_H
#define VKRT_LAYERPRESENTER_H

#include "Presenter.h"

#include <vulkan/vulkan.hpp>

namespace vkrt
{
    class LayerPresenter
    :
        public vkrt::Presenter
    {
        public:
            LayerPresenter( vks::DevicePtr inDevice, vk::Extent2D inExtent );
            ~LayerPresenter();

        public:
            void Prepare( vkrt::RenderEnvironment const & inEnvironment ) override;
            void Draw( vkrt::RenderEnvironment const & inEnvironment ) override;
            void AddPresenter( std::shared_ptr< vkrt::Presenter > inPresenter );

        private:
            void InitializeRenderPass();
            void InitializePipeline( vk::RenderPass inRenderPass );

        private:
            vks::DevicePtr mDevice;
            vk::Extent2D mExtent;

            // Render prepare info
            vk::RenderPass mRenderPass;
            struct PresenterContext
            {
                vks::Image mImage;
                vk::ImageView mImageView;
                vk::Framebuffer mFramebuffer;
                vk::Sampler mSampler;
                std::shared_ptr< vkrt::Presenter > mPresenter;
            };
            std::vector< PresenterContext > mPresenterContexts;

            // Draw info
            vks::PipelinePtr mPipeline;
    };
}

#endif //VKRT_LAYERPRESENTER_H
