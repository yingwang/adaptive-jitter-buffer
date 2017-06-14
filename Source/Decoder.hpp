//
//  Decoder.hpp
//  AJB
//
//  Created by Ying Wang on 18/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Decoder_hpp
#define Decoder_hpp

#include "IDecoder.hpp"

namespace AJB {
    
class Decoder : public IDecoder
{
public:
    int DecodeFrame(const char* buffer, int length, char* outputBuffer);
    Decoder() {}
};

}

#endif /* Decoder_hpp */
