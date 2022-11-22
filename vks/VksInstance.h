//
// Created by magnias on 7/7/22.
//

#ifndef VKRT_VKSINSTANCE_H
#define VKRT_VKSINSTANCE_H

#include <vulkan/vulkan.hpp>

namespace vks
{
    class Instance
    {
        public:
            Instance();
            ~Instance();

        private:
            void CreateInstance();

            VkInstance mInstance;
    };
}


#endif //VKRT_VKSINSTANCE_H
