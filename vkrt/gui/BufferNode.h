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
#include "InputAttribute.h"
#include "OutputAttribute.h"
#include "vks/core/Image.h"

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
                gui::Node( -1 ),
                mInput( inNodeContext->CreateInputAttribute< vks::Image >
                (
                    [ this ]( const std::optional< vks::Image >& ){ mOutput->Update(); },
                    [ this ]( const std::optional< vks::Image >& ){ mOutput->Update(); },
                    [ this ](){ mOutput->Update(); }
                ) ),
                mOutput( inNodeContext->CreateOutputAttribute< vks::Image >
                (
                    [ this ](){ return mInput->GetResource(); } )
                )
            {
                LinkAttribute( mInput );
                LinkAttribute( mOutput );
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
            std::shared_ptr< vkrt::gui::InputAttribute< vks::Image > > mInput;
            std::shared_ptr< vkrt::gui::OutputAttribute< vks::Image > > mOutput;
    };
}

#endif //VKRT_BUFFERNODE_H
