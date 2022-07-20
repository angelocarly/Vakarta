//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H


#include "vks/vks_device.h"

class Engine
{

public:

    Engine();
    ~Engine();

    void update();
    void render();

private:
    VksDevice device;
};


#endif //VKRT_ENGINE_H
