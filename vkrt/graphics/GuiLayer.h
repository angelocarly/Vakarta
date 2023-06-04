//
// Created by magnias on 6/4/23.
//

#ifndef VKRT_GUILAYER_H
#define VKRT_GUILAYER_H

#include "Layer.h"

#include "vks/render/ForwardDecl.h"

namespace vkrt
{
    class GuiLayer
    :
        public vkrt::Layer
    {
        public:
            GuiLayer( vks::DevicePtr inDevice );
            ~GuiLayer();

        public:
            void Render( vkrt::RenderEnvironment & inRenderEnvironment ) override;

        private:
            vks::DevicePtr mDevice;
            vk::RenderPass mRenderPass;
    };
}

#endif //VKRT_GUILAYER_H
