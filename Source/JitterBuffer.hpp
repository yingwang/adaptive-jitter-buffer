//
//  JitterBuffer.hpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef JitterBuffer_hpp
#define JitterBuffer_hpp

#include "IJitterBuffer.hpp"
#include "FrameList.hpp"
#include "Clock.hpp"
#include "IDecoder.hpp"
#include "IRenderer.hpp"
#include <thread>
#include <queue>

namespace AJB {
    
class JitterBuffer : public IJitterBuffer
{
private:
    std::unique_ptr<Clock> _clock;
    std::unique_ptr<std::thread> _writerThread;
    std::shared_ptr<IDecoder> _decoder;
    std::shared_ptr<IRenderer> _renderer;
    
    /* frame rate */
    double _frameRate;
    double _lastFrameRate;
    double _avgFrameRate;
    bool _stop;
    int _lastDecodableFrameNumber;
    std::queue<long> _decodedFrameTime;

    FrameList _decodableFrames;
    FrameList _completeFrames;
    FrameList _incompleteFrames;
    
    const double SPEED_FAST = 0.3;
    const double SPEED_NOR = 0.8;
    const double SPEED_SLOW = 1.5;
    const int BUFFER_SIZE_MAX = 6;
    const int BUFFER_SIZE_NOR = 3;
    const int BUFFER_SIZE_MIN = 1;
    const int NUM_FRAME_TIME_HISTORY = 60;
    const int TIME_INTERVAL = 10000; // 10 ms
    
public:
    JitterBuffer(IDecoder* decoder, IRenderer* renderer);
 
    void ReceivePacket(const char* buffer, int length, int frameNumber,
                       int fragmentNumber, int numFragmentsInThisFrame);
    
    double FrameRate() const { return _frameRate; }
    double AverageFrameRate() const { return _avgFrameRate; }
    double BufferSize() const { return _decodableFrames.size(); }
    int LastDecodableFrameNumber() const { return _lastDecodableFrameNumber; }
    void Start();
    void Stop();
    void SendFrameDownstream();
    ~JitterBuffer();
};

}

#endif /* JitterBuffer_hpp */
