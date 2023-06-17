/**
 * TestPresenter.h
 *
 * @file	TestPresenter.h
 * @author	Angelo Carly
 * @date	11/06/2023
 *
 */

#ifndef VKRT_TESTPRESENTER_H
#define VKRT_TESTPRESENTER_H

#include "vkrt/graphics/Presenter.h"
#include "RenderEnvironment.h"

namespace vkrt
{
    class TestPresenter
    :
        public vkrt::Presenter
    {
        public:
            TestPresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight );
            ~TestPresenter();

            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment ) override;

        private:
            void InitializePipeline( vk::RenderPass const inRenderPass );

        private:
            vks::DevicePtr mDevice;
            vks::PipelinePtr mPipeline;

    };
}

#endif //VKRT_TESTPRESENTER_H
