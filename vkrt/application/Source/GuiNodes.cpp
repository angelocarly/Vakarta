/**
 * GuiNodes.cpp
 *
 * @file	GuiNodes.cpp
 * @author	Angelo Carly
 * @date	22/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "vkrt/application/GuiNodes.h"
#include "imnodes.h"

vkrt::GuiNodes::GuiNodes()
{

}

void
vkrt::GuiNodes::Draw()
{
    ImNodes::BeginNodeEditor();

    ImNodes::BeginNode( 1 );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("output node");
        ImNodes::EndNodeTitleBar();

        const int output_attr_id = 4;
        ImNodes::BeginOutputAttribute( output_attr_id );
        // in between Begin|EndAttribute calls, you can call ImGui
        // UI functions
        ImGui::Text( "output pin" );
        ImNodes::EndOutputAttribute();
    }
    ImNodes::EndNode();

    ImNodes::BeginNode( 2 );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("simple node :)");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(2);
        ImGui::Text("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(3);
        ImGui::Indent(40);
        ImGui::Text("output");
        ImNodes::EndOutputAttribute();
    }
    ImNodes::EndNode();

//    ImNodes::Link( 0, 2, 3 );

    ImNodes::EndNodeEditor();
}
