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
            TestPresenter( vks::DevicePtr inDevice );
            ~TestPresenter();

        private:
            void InitializePipeline( vk::RenderPass inRenderPass );

        private:
            vks::DevicePtr mDevice;
            vks::PipelinePtr mPipeline;

        public:
            void Prepare( vkrt::RenderEnvironment const & inEnvironment ) override;
            void Draw( vkrt::RenderEnvironment const & inEnvironment ) override;
    };
}

#endif //VKRT_TESTPRESENTER_H
