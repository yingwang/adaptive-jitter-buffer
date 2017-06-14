//
//  Frame.hpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Frame_hpp
#define Frame_hpp

#include "Packet.hpp"
#include <list>

namespace AJB {
    
enum FrameStateEnum
{
    Incomplete,
    Complete,
    Decodable
};
    
class Frame
{
private:
    const int _numFragmentsInThisFrame;
    int _numFragmentsReceived;
    const int _frameNumber;
    char* _buffer;
    int _bufferSize;
    FrameStateEnum _state;
    PacketList _packetList;

public:
    Frame(int FrameNumber, int numFragmentsInThisFrame);
    ~Frame();
    int NumFragmentsReceived() const { return NumFragmentsReceived(); };
    int FrameNumber() const { return _frameNumber; };
    FrameStateEnum State() const { return _state; };
    const char* Buffer() const { return _buffer; }
    const int BufferSize() const { return _bufferSize; }
    FrameStateEnum InsertPacket(std::shared_ptr<Packet>);
    void MakeFrameComplete();
};
}

#endif /* Frame_hpp */
