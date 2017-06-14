//
//  Frame.cpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Frame.hpp"

namespace AJB {
    
Frame::Frame(int frameNumber, int numFragmentsInThisFrame)
    :_state(Incomplete), _numFragmentsReceived(0), _bufferSize(0),
    _numFragmentsInThisFrame(numFragmentsInThisFrame), _frameNumber(frameNumber){}

FrameStateEnum Frame::InsertPacket(std::shared_ptr<Packet> packet)
{
    bool inserted = false;
    // first packet in frame
    if (_packetList.size() == 0)
    {
        _packetList.insert(_packetList.begin(), packet);
        inserted = true;
    }
    else
    {
        for (auto rit = _packetList.rbegin(); rit != _packetList.rend(); rit++)
        {
            // Discard redundant packets
            if ((*rit)->PacketNumber() == packet->PacketNumber())
            {
                inserted = true;
                return _state;
            }
            if ((*rit)->PacketNumber() < packet->PacketNumber())
            {
                PacketList::iterator it = rit.base();
                _packetList.insert(it, packet);
                inserted = true;
                break;
            }
        }
        if (!inserted)
        {
            _packetList.insert(_packetList.begin(), packet);
        }
    }
    
    _bufferSize += packet->PayloadSize();
    _numFragmentsReceived++;
    if (_numFragmentsReceived == _numFragmentsInThisFrame)
    {
        MakeFrameComplete();
    }
    
    return _state;
}

void Frame::MakeFrameComplete()
{
    _buffer = new char[_bufferSize];
    char* bufferPtr = _buffer;
    for (auto it = _packetList.begin(); it != _packetList.end(); it++)
    {
        memcpy(_buffer, (*it)->Payload(), (*it)->PayloadSize());
        //delete *it;
        bufferPtr += (*it)->PayloadSize();
    }
    _packetList.clear();
    _state = Complete;
}

Frame::~Frame()
{
    for (auto it = _packetList.begin(); it != _packetList.end(); it++)
    {
        //delete *it;
    }
    if (_buffer != NULL)
    {
        delete[] _buffer;
    }
    _buffer = nullptr;
}

}
