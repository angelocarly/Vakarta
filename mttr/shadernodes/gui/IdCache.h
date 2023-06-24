//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_IDCACHE_H
#define VKRT_IDCACHE_H

#include <cstdio>
#include <vector>

namespace vkrt::gui
{
    class IdCache
    {
        public:
            IdCache() = default;
            ~IdCache() = default;

            std::size_t AddId();
            void RemoveId( std::size_t inId );

        private:
            std::vector< std::size_t> mIds;
            std::vector< std::size_t > mUnusedIds;
    };
}

#endif //VKRT_IDCACHE_H
