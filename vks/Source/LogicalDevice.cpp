//
// Created by Angelo Carly on 21/11/2022.
//

#include "vks/LogicalDevice.h"

class vks::LogicalDevice::Impl
{
    public:
        vk::Device mDevice;
};

vks::LogicalDevice::LogicalDevice()
{

}

vks::LogicalDevice::~LogicalDevice()
{}
