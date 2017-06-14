//
//  Packet.hpp
//  AJB
//
//  Created by Ying Wang on 18/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Packet_hpp
#define Packet_hpp

#include <list>

namespace AJB {
    
class Packet
{
private:
    const int _frameNumber;
    const int _packetNumber;
    char* _payload;
    const int _payloadSize;
public:
    Packet();
    ~Packet();
    Packet(const char* payload,
           const int payloadSize,
           const int frameNumber,
           const int packetNumber);
    int PacketNumber() const { return _packetNumber; }
    int PayloadSize() const { return _payloadSize; }
    char* Payload() const { return _payload; }
};

typedef std::list<std::shared_ptr<Packet>> PacketList;
    
}

#endif /* Packet_hpp */
