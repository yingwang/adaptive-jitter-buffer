//
//  FrameList.hpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef FrameList_hpp
#define FrameList_hpp

#include <map>
#include "Frame.hpp"

namespace AJB {
    
class FrameNumberLessThan
{
public:
    bool operator()(int frameNumber1, int frameNumber2) const
    {
        return frameNumber1 < frameNumber2;
    }
};

class FrameList
    : public std::map<int, std::shared_ptr<Frame>, FrameNumberLessThan>
{
public:
    void InsertFrame(std::shared_ptr<Frame> frame);
    std::shared_ptr<Frame> PopFrame(int frameNumber);
    std::shared_ptr<Frame> Front() const;
    std::shared_ptr<Frame> Back() const;
};

}

#endif /* FrameList_hpp */
