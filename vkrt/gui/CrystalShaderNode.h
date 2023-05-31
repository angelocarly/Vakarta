/**
 * CrystalShaderNode.h
 *
 * @file	CrystalShaderNode.h
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_CRYSTALSHADERNODE_H
#define VKRT_CRYSTALSHADERNODE_H

#include "Node.h"
#include "vks/render/ForwardDecl.h"
#include "vkrt/graphics/CrystalViewShader.h"
#include "vks/render/OffScreenRenderPass.h"

namespace vkrt::gui
{
    class CrystalShaderNode
    :
        public Node
    {
        public:
            CrystalShaderNode( vkrt::gui::NodeContextPtr inContext );
            ~CrystalShaderNode();

        public:
            void Draw() override;

        private:
            std::shared_ptr< vks::OffScreenRenderPass > mRenderPass;

        private:
            std::shared_ptr< vkrt::gui::InputAttribute< float > > mInput;
            std::shared_ptr< vkrt::graphics::CrystalViewShader > mShader;
    };
}


#endif //VKRT_CRYSTALSHADERNODE_H
