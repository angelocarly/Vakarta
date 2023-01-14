//
// Created by Angelo Carly on 11/01/2023.
//

#include "vkrt/graphics/LinePipeline.h"

class vkrt::LinePipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
        ~Impl();

    public:
        vks::DevicePtr mDevice;
        vks::RenderPassPtr mRenderPass;

};

vkrt::LinePipeline::Impl::Impl( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass )
:
    mDevice( inDevice ),
    mRenderPass( inRenderPass )
{

}

vkrt::LinePipeline::Impl::~Impl()
{

}

//======================================================================================================================

vkrt::LinePipeline::LinePipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass )
:
    mImpl( new Impl( inDevice, inRenderPass ) )
{

}

vkrt::LinePipeline::~LinePipeline()
{

}
