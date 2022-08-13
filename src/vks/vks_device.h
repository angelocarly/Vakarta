//
// Created by magnias on 7/7/22.
//

#ifndef VKRT_VKS_DEVICE_H
#define VKRT_VKS_DEVICE_H

#include <vulkan/vulkan.h>

class VksDevice
{

public:
    VksDevice();
    ~VksDevice();

private:
    void createInstance();
    void destroyInstance();

    VkInstance instance;
};


#endif //VKRT_VKS_DEVICE_H
