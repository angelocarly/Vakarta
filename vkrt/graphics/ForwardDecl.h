/**
 * ForwardDecl.h
 *
 * @file	ForwardDecl.h
 * @author	Angelo Carly
 * @date	13/05/2023
 *
 */

#ifndef Vkrt_Graphics_ForwardDecl_h_Angelo
#define Vkrt_Graphics_ForwardDecl_h_Angelo

#include <memory>

namespace vkrt::graphics
{
    class CrystalViewShader;
    using CrystalViewShaderPtr = std::shared_ptr< CrystalViewShader >;

    class FilterShader;
    using FilterShaderPtr = std::shared_ptr< FilterShader >;
}

#endif
