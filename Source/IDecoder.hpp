//
//  IDecoder.hpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef IDecoder_h
#define IDecoder_h

namespace AJB {

class IDecoder
{
public:
    /*
     Returns the size of the data written to the outputBuffer, will be no more than 1mb.
     */
    virtual int DecodeFrame(const char* buffer, int length, char* outputBuffer) = 0;
    virtual ~IDecoder() {}
};

}
    
#endif /* IDecoder_h */
