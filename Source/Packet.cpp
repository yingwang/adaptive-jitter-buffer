//
//  Packet.cpp
//  AJB
//
//  Created by Ying Wang on 18/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Packet.hpp"

namespace AJB {
    
Packet::Packet(const char* payload, const int payloadSize, const int frameNumber, const int packetNumber)
    : _payloadSize(payloadSize),
      _frameNumber(frameNumber),
      _packetNumber(packetNumber)
{
    _payload = new char[payloadSize];
    memcpy(_payload, payload, payloadSize);
}

Packet::~Packet()
{
    delete[] _payload;
    _payload = nullptr;
}
    
}
