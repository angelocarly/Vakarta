//
// Created by magnias on 4/16/23.
//

#include "vks/assets/ImageResource.h"

#include <cstdio>
#include <span>
#include <cstdint>
#include <spdlog/spdlog.h>

vks::ImageResource::ImageResource( std::size_t inWidth, std::size_t inHeight, Format inFormat, std::vector< std::uint8_t > inData )
:
    mWidth( inWidth ),
    mHeight( inHeight ),
    mFormat( inFormat ),
    mData( std::move( inData ) )
{
    std::size_t theChannels = 4;
    switch( inFormat )
    {
        case RGBA:
            theChannels = 4;
            break;
        case RGB:
            theChannels = 3;
            break;
        default:
            throw new std::runtime_error( "ImageResource::ImageResource: Unknown format" );
    }
    if( mData.size() != inWidth * inHeight * theChannels )
    {
        spdlog::error( "ImageResource::ImageResource: Data size does not match width, height, and channels" );
        throw new std::runtime_error( "ImageResource::ImageResource: Data size does not match width, height, and channels" );
    }
}

vks::ImageResource::~ImageResource()
{

}
