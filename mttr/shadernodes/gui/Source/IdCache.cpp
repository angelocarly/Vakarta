//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/IdCache.h"

#include <stdexcept>

std::size_t
vkrt::gui::IdCache::AddId()
{
    if( mUnusedIds.empty() )
    {
        // Allocate a new id
        auto theId = mIds.size();
        mIds.push_back( theId );
        return theId;
    }
    else
    {
        // Reuse an old id
        auto theId = mUnusedIds.back();
        mUnusedIds.pop_back();
        mIds.push_back( theId );
        return theId;
    }
}

void
vkrt::gui::IdCache::RemoveId( std::size_t inId )
{
    for( int i = 0; i < mIds.size(); ++i )
    {
        if( mIds[i] == inId )
        {
            mIds.erase( mIds.begin() + i );
            mUnusedIds.push_back( inId );
            return;
        }
    }

    throw new std::runtime_error( "Id not found" );
}
