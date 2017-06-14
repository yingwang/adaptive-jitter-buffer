//
//  JitterBuffer.cpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "JitterBuffer.hpp"
#include "Packet.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <iomanip>

namespace AJB {
    
JitterBuffer::JitterBuffer(IDecoder* decoder, IRenderer* renderer) : IJitterBuffer(decoder, renderer)
{
    _clock = std::unique_ptr<Clock>(new Clock());
    _lastDecodableFrameNumber = -1;
    _frameRate = 1.0f;
    _lastFrameRate = 1.0f;
    _avgFrameRate = 1.0f;
    _stop = false;
    _decoder = std::unique_ptr<IDecoder>(decoder);
    _renderer = std::unique_ptr<IRenderer>(renderer);
    _decodedFrameTime.push(0);
    _writerThread.reset(new std::thread(&JitterBuffer::SendFrameDownstream, this));
}

JitterBuffer::~JitterBuffer()
{
}

void JitterBuffer::Start()
{
    _clock->Start();
}

void JitterBuffer::Stop()
{
    _stop = true;
    _writerThread->join();
}

void JitterBuffer::ReceivePacket(const char* buffer, int length, int frameNumber,
                   int fragmentNumber, int numFragmentsInThisFrame)
{
    // Discard redundant packet
    if (frameNumber <= _lastDecodableFrameNumber)
    {
        return;
    }
    // Packet for an incomplete frame
    std::shared_ptr<Frame> frame = std::shared_ptr<Frame>(_incompleteFrames.PopFrame(frameNumber));
    // Packet for a new frame
    if (frame == NULL)
    {
        frame = std::unique_ptr<Frame>(new Frame(frameNumber, numFragmentsInThisFrame));
    }

    std::shared_ptr<Packet> packet = std::shared_ptr<Packet>(new Packet(buffer, length, frameNumber, fragmentNumber));
    FrameStateEnum state = frame->InsertPacket(packet);
    
    if (state == Complete)
    {
        // Find frames ready for decoding
        if (frame->FrameNumber() == (_lastDecodableFrameNumber + 1))
        {
            while (frame)
            {
                _decodableFrames.InsertFrame(frame);
                _lastDecodableFrameNumber++;
                frame = _completeFrames.PopFrame(_lastDecodableFrameNumber + 1);
            }
        }
        else
        {
            _completeFrames.InsertFrame(frame);
        }
    }
    else
    {
        _incompleteFrames.InsertFrame(frame);
    }
}

void JitterBuffer::SendFrameDownstream()
{
    while (!_stop)
    {
        usleep(TIME_INTERVAL);
        long currentTime = _clock->TimeInMilliseconds();
        if (BufferSize() < BUFFER_SIZE_MIN)
        {
            continue;
        }
        else if (BufferSize() < BUFFER_SIZE_NOR)
        {
            if ((currentTime - _decodedFrameTime.back()) < (1000 * SPEED_SLOW / _avgFrameRate))
            {
                continue;
            }
        }
        else if (BufferSize() < BUFFER_SIZE_MAX)
        {
            if ((currentTime - _decodedFrameTime.back()) < (1000 * SPEED_NOR / _avgFrameRate) &&
                (currentTime - _decodedFrameTime.back()) < (1000 * SPEED_NOR  / _lastFrameRate))
            {
                continue;
            }
        }
        else if (BufferSize() >= BUFFER_SIZE_MAX)
        {
            if ((currentTime - _decodedFrameTime.back()) < (1000 * SPEED_FAST/ _lastFrameRate) &&
                (currentTime - _decodedFrameTime.back()) < (1000 * SPEED_FAST  / _avgFrameRate))
            {
                continue;
            }
        }

        std::shared_ptr<Frame> frame = std::shared_ptr<Frame>(_decodableFrames.Front());
        if (frame == NULL)
        {
            continue;
        }
        _decodableFrames.PopFrame(frame->FrameNumber());
        char* decodedbuffer;
        int decodedBufferSize = _decoder->DecodeFrame(frame->Buffer(), frame->BufferSize(), decodedbuffer);
        _renderer->RenderFrame(decodedbuffer, decodedBufferSize);

        _frameRate = 1000.0 / (currentTime - _decodedFrameTime.back());
        _lastFrameRate = _frameRate;
        _decodedFrameTime.push(currentTime);
        _avgFrameRate = _decodedFrameTime.size() * 1000.0 / (currentTime - _decodedFrameTime.front());
        
        if (_decodedFrameTime.size() >= NUM_FRAME_TIME_HISTORY)
        {
            _decodedFrameTime.pop();
        }
    }
}

}
