//
//  JitterBufferUnitTest.cpp
//  AJB
//
//  Created by Ying Wang on 18/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "JitterBuffer.hpp"
#include "Decoder.hpp"
#include "Renderer.hpp"
#include "Clock.hpp"
#include <iostream>
#include <unistd.h>
#include <random>
#include <string>

using namespace AJB;

std::string random_string(int length);
const int TIME_INTERVAL = 10000;
const int PACKET_SIZE = 1500;
const int TEST_LENGTH = 1000;
const int CONGESTION_TIME = 1000000; // 1 sec

TEST_CASE( "Test Clock" )
{
    Clock clock;
    SECTION("starting clock") {
        INFO("starting clock")
        clock.Start();
        long time = clock.TimeInMilliseconds();
        CAPTURE(time);
        REQUIRE(time >= 0);
        usleep(TIME_INTERVAL);
        CHECK(clock.TimeInMilliseconds() - time > 9);
    }
}

TEST_CASE("Test JitterBuffer", "Normal Condition")
{
    JitterBuffer* jitterBuffer = new JitterBuffer(new Decoder(), new Renderer());
    jitterBuffer->Start();
    int frameNumber = 0;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> timeDist(20000, 200000);
    std::uniform_int_distribution<int> fragmentDist(1, 10);
    while (frameNumber < TEST_LENGTH)
    {
        usleep(timeDist(generator));
        int fragmentsInThisFrame = fragmentDist(generator);
        std::string s = random_string(PACKET_SIZE);
        for (int i = 0; i < fragmentsInThisFrame; i++)
        {
            jitterBuffer->ReceivePacket(s.c_str(), static_cast<int>(s.length()), frameNumber, i, fragmentsInThisFrame);
        }
        frameNumber++;
        REQUIRE(jitterBuffer->FrameRate() > 0);
        REQUIRE(jitterBuffer->AverageFrameRate() > 0);
        REQUIRE(jitterBuffer->BufferSize() >= 0);
        CHECK(jitterBuffer->FrameRate() < (jitterBuffer->AverageFrameRate() * 3));
        CHECK(jitterBuffer->FrameRate() > (jitterBuffer->AverageFrameRate() * 0.3));
        CHECK(jitterBuffer->BufferSize() < 10);
        std::cout << "fps: ";
        std::cout << std::setw(8);
        std::cout << jitterBuffer->FrameRate();
        std::cout << "  avg fps: ";
        std::cout << std::setw(8);
        std::cout << jitterBuffer->AverageFrameRate();
        std::cout << "  Buf: ";
        std::cout << std::setw(2);
        std::cout << jitterBuffer->BufferSize() << std::endl;
    }
    jitterBuffer->Stop();
}

TEST_CASE("Test JitterBuffer - Catch UP", "Network Congestion")
{
    JitterBuffer* jitterBuffer = new JitterBuffer(new Decoder(), new Renderer());
    jitterBuffer->Start();
    int frameNumber = 0;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> timeDist(20000, 200000);
    std::uniform_int_distribution<int> fragmentDist(1, 10);
    while (frameNumber < TEST_LENGTH)
    {
        usleep(timeDist(generator));
        if (frameNumber % 50 == 0)
        {
            usleep(CONGESTION_TIME);
        }
        int fragmentsInThisFrame = fragmentDist(generator);
        std::string s = random_string(PACKET_SIZE);
        for (int i = 0; i < fragmentsInThisFrame; i++)
        {
            jitterBuffer->ReceivePacket(s.c_str(), static_cast<int>(s.length()), frameNumber, i, fragmentsInThisFrame);
        }
        frameNumber++;
        REQUIRE(jitterBuffer->FrameRate() > 0);
        REQUIRE(jitterBuffer->AverageFrameRate() > 0);
        REQUIRE(jitterBuffer->BufferSize() >= 0);
        CHECK(jitterBuffer->FrameRate() < (jitterBuffer->AverageFrameRate() * 3));
        CHECK(jitterBuffer->FrameRate() > (jitterBuffer->AverageFrameRate() * 0.3));
        CHECK(jitterBuffer->BufferSize() < 10);
        std::cout << "fps: ";
        std::cout << std::setw(8);
        std::cout << jitterBuffer->FrameRate();
        std::cout << "  avg fps: ";
        std::cout << std::setw(8);
        std::cout << jitterBuffer->AverageFrameRate();
        std::cout << "  Buf: ";
        std::cout << std::setw(2);
        std::cout << jitterBuffer->BufferSize() << std::endl;
    }
    jitterBuffer->Stop();
}

std::string random_string(int length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}
