# adaptive-jitter-buffer

#### Adaptive jitter buffer implementation

#### Classes

• JitterBuffer: takes care of receiving packets, pushes decodable frames downstream and maintains three different FrameLists.

• FrameList: list of frames classified by their state. Use std::map to achieve fast index search.

• Frame: contains a buffer for reassemble the packets.

• PacketList: use std::list for quick insertion.

• Packet: contains the payload.

• Clock: for timing.

#### Supported functions

• Reassemble packets into frames

• Pass assembled frames to decoder

• Send decoded frames to the render

• Remove duplicate packets

• Playout delay decrease in an appropriate rate after a congestion 

• Adaptive buffer size

• Use separate thread for playout

• Simple unit-tests

#### Adaptive algorithm

For real-time video communication, the jitter buffer size should not go over 200ms. Assume frame rate is 30 fps, the ideal buffer size is between 1 - 6 frames. In this implementation, the timestamp of the last N decoded frames are recorded in order to estimate an average frame rate.

The playout speed is dependent on the jitter buffer size,
when the buffer is under-run / low, the jitter buffer slows down the playout rate and increase its buffer size;
when the buffer is over-run / full, the jitter buffer speeds up the playout rate and decrease its buffer size. In order to catch up smoothly, the frame rate shall not increase too fast, thus it is limited to around 3.5x of the last frame playout rate. In this way, after a network congestion the frame rate will climb back slowly.

#### Limitations

The current implementation uses frame rate to control the playout, it will be better to use bitrate. Also the buffer size shall be estimated by time instead of buffered frames.

Adaptive algorithm still has room for improvements.

The unit test just covers a couple simple cases.
