//
//  FrameList.cpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "FrameList.hpp"

namespace AJB {
    
typedef std::pair<int, std::shared_ptr<Frame>> FrameListPair;

void FrameList::InsertFrame(std::shared_ptr<Frame> frame)
{
    insert(rbegin().base(), FrameListPair(frame->FrameNumber(), frame));
}

std::shared_ptr<Frame> FrameList::PopFrame(int frameNumber)
{
    auto it = find(frameNumber);
    if (it == end())
    {
        return NULL;
    }
    std::shared_ptr<Frame> frame = it->second;
    erase(it);
    return frame;
}

std::shared_ptr<Frame> FrameList::Front() const
{
    if (size() == 0)
    {
        return NULL;
    }
    return begin()->second;
}
    
}
