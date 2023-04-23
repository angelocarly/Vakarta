//
// Created by magnias on 4/16/23.
//

#ifndef VKRT_IMAGERESOURCE_H
#define VKRT_IMAGERESOURCE_H

#include <cstdio>
#include <vector>
#include <span>
#include <cstdint>

namespace vks
{
    class ImageResource
    {
        public:
            ImageResource( std::size_t inWidth, std::size_t inHeight, std::size_t inChannels, std::vector< std::uint8_t > inData );
            ~ImageResource();

            std::size_t GetWidth() { return mWidth; }
            std::size_t GetHeight() { return mHeight; }
            std::size_t GetChannels() { return mChannels; }
            std::size_t GetDataSize() { return mData.size(); }
            std::vector< std::uint8_t > GetData() { return mData; };

        private:
            std::size_t mWidth;
            std::size_t mHeight;
            std::size_t mChannels;
            std::vector< std::uint8_t > mData;
    };
}

#endif //VKRT_IMAGERESOURCE_H
