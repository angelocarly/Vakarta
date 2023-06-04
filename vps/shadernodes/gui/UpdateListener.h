//
// Created by magnias on 5/1/23.
//

#ifndef VKRT_UPDATELISTENER_H
#define VKRT_UPDATELISTENER_H

namespace vkrt::gui
{
    class UpdateListener
    {
        public:
            virtual void Update() = 0;
    };
}

#endif //VKRT_UPDATELISTENER_H
