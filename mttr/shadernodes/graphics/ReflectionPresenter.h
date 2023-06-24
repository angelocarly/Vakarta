/**
 * ReflectionPresenter.h
 *
 * @file	ReflectionPresenter.h
 * @author	Angelo Carly
 * @date	10/04/2023
 *
 *
 */

#ifndef VKRT_REFLECTIONPRESENTER_H
#define VKRT_REFLECTIONPRESENTER_H

#include "Presenter.h"
#include "vks/render/ForwardDecl.h"

namespace vkrt
{
    class ReflectionPresenter
        : public Presenter
    {
        public:
            ReflectionPresenter( vks::VulkanSessionPtr inSession );
            ~ReflectionPresenter();

            vk::DescriptorSet GetImage() override;

        private:
            vks::DevicePtr mDevice;
            vks::ImagePtr mImage;
            vk::ImageView mImageView;
            vk::Sampler mSampler;
            vk::DescriptorSet mImageDescriptorSet;
    };
}

#endif //VKRT_REFLECTIONPRESENTER_H
