//
//  Clock.cpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Clock.hpp"

namespace AJB {
    
void Clock::Start()
{
    _startTimeVal = CurrentTimeVal();
}

long Clock::TimeInMilliseconds() const
{
    return (CurrentTimeVal().tv_sec * 1000000 + CurrentTimeVal().tv_usec - _startTimeVal.tv_sec * 1000000 - _startTimeVal.tv_usec) / 1000;
}

timeval Clock::CurrentTimeVal() const
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}
}
