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

#include "vkrt/graphics/ForwardDecl.h"

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
            void Draw( vkrt::RenderEnvironment const & inEnvironment ) override;
            void AddPresenter( vkrt::PresenterPtr inPresenter );

        private:
            void InitializePipeline( vk::RenderPass inRenderPass );

        private:
            vks::DevicePtr mDevice;
            vk::Extent2D mExtent;
            vk::DescriptorSetLayout mDescriptorLayout;

            // Render prepare info
            vks::PipelinePtr mPipeline;
            std::vector< PresenterPtr > mPresenters;
    };
}

#endif //VKRT_LAYERPRESENTER_H
