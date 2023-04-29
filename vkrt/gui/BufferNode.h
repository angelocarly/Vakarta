/**
 * BufferNode.h
 *
 * @file	BufferNode.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_BUFFERNODE_H
#define VKRT_BUFFERNODE_H

#include "Node.h"
#include "imnodes.h"

namespace vkrt::gui
{
    class BufferNode
    :
    public gui::Node
    {
        public:
            BufferNode( vkrt::gui::NodeContextPtr inNodeContext )
            :
                gui::Node( inNodeContext ),
                mInput( inNodeContext->AddInputAttribute() ),
                mOutput( inNodeContext->AddOutputAttribute( [ this ](){ return mInput->GetResource(); } ) )
            {
            }
            ~BufferNode() {};

            void Draw() override
            {
                ImNodes::BeginNode( GetId() );
                {
                    ImNodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted( "Buffer" );
                    ImNodes::EndNodeTitleBar();

                    ImNodes::BeginInputAttribute( mInput->mId );
                    ImGui::Text( "input" );
                    ImNodes::EndInputAttribute();

                    ImNodes::BeginOutputAttribute( mOutput->mId );
                    ImGui::Indent( 40 );
                    ImGui::Text( "output" );
                    ImNodes::EndOutputAttribute();
                }
                ImNodes::EndNode();
            }

        private:
            std::shared_ptr< vkrt::gui::InputAttribute< vk::DescriptorSet > > mInput;
            std::shared_ptr< vkrt::gui::OutputAttribute< vk::DescriptorSet > > mOutput;
    };
}

#endif //VKRT_BUFFERNODE_H
