//
// Created by magnias on 4/16/23.
//

#include "vks/assets/ImageResource.h"

#include <cstdio>
#include <span>
#include <cstdint>
#include <spdlog/spdlog.h>

vks::ImageResource::ImageResource( std::size_t inWidth, std::size_t inHeight, std::size_t inChannels, std::vector< std::uint8_t > inData )
:
    mChannels( inChannels ),
    mWidth( inWidth ),
    mHeight( inHeight ),
    mData( std::move( inData ) )
{
    if( mData.size() != inWidth * inHeight * inChannels )
    {
        spdlog::error( "ImageResource::ImageResource: Data size does not match width, height, and channels" );
        throw new std::runtime_error( "ImageResource::ImageResource: Data size does not match width, height, and channels" );
    }
}

vks::ImageResource::~ImageResource()
{

}
