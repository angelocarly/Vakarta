/**
 * ResourceSink.h
 *
 * @file	ResourceSink.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 *
 */

#ifndef VKRT_INPUTATTRIBUTE_H
#define VKRT_INPUTATTRIBUTE_H

#include "GuiAttribute.h"
#include "NodeContext.h"
#include "ResourceProvider.h"
#include "ForwardDecl.h"
#include "UpdateListener.h"

#include <stdexcept>
#include <functional>

namespace vkrt::gui
{
    template< typename T >
    class InputAttribute
    :
        public GuiAttribute,
        public UpdateListener
    {
        public:
            InputAttribute( std::function< void ( std::optional< T > ) > inConnectCallback, std::function< void ( std::optional< T > ) > inUpdateCallback, std::function< void( void ) > inDisconnectCallback );
            ~InputAttribute();

            void Connect( std::shared_ptr< vkrt::gui::ResourceProvider< T > > inProvider );
            void Update();
            void Disconnect();
            bool IsConnected() const;
            std::optional< T > GetResource() const;

        private:
            std::shared_ptr< vkrt::gui::ResourceProvider< T > > mProvider;
            std::function< void( std::optional< T > ) > mConnectCallback;
            std::function< void( std::optional< T > ) > mUpdateCallback;
            std::function< void( void ) > mDisconnectCallback;
    };

    template< typename T >
    std::optional< T > InputAttribute< T >::GetResource() const
    {
        if( IsConnected() ) return mProvider->GetResource();
        return std::optional< T >();
    }

    template< typename T >
    void InputAttribute< T >::Disconnect()
    {
        mProvider = nullptr;
        mDisconnectCallback();
    }

    template< typename T >
    bool InputAttribute< T >::IsConnected() const
    {
        return mProvider != nullptr;
    }

    template< typename T >
    InputAttribute< T >::InputAttribute( std::function< void ( std::optional< T > ) > inConnectCallback, std::function< void ( std::optional< T > ) > inUpdateCallback, std::function< void( void ) > inDisconnectCallback )
    :
        GuiAttribute( -1, GuiAttribute::kInput, GuiAttribute::ResourceType::kImage ),
        mConnectCallback( inConnectCallback ),
        mUpdateCallback( inUpdateCallback ),
        mDisconnectCallback( inDisconnectCallback )
    {

    }

    template< typename T >
    InputAttribute< T >::~InputAttribute()
    {

    }

    template< typename T >
    void
    vkrt::gui::InputAttribute< T >::Connect( std::shared_ptr< vkrt::gui::ResourceProvider< T > > inProvider )
    {
        mProvider = std::move( inProvider );
        mConnectCallback( mProvider->GetResource() );
    }

    template< typename T >
    void
    vkrt::gui::InputAttribute< T >::Update()
    {
        mUpdateCallback( mProvider->GetResource() );
    }
}

#endif //VKRT_INPUTATTRIBUTE_H
