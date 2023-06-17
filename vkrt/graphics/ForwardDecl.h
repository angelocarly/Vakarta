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

namespace vkrt
{

    class Layer;
    using LayerPtr = std::shared_ptr<Layer>;

    class Presenter;
    using PresenterPtr = std::shared_ptr< Presenter >;
}

#endif
