#pragma once

#include <iostream>
#include <chrono>

static std::chrono::steady_clock::time_point speedCPUTesterBegin;
static std::chrono::steady_clock::time_point speedCPUTesterEnd;

static void beginCPUSpeedTest(){
    speedCPUTesterBegin = std::chrono::steady_clock::now();
}

static void endCPUSpeedTest(){
    speedCPUTesterEnd = std::chrono::steady_clock::now();
}

static void printCPUNanoSeconds(){
    std::cout << "CPU = " << std::chrono::duration_cast<std::chrono::nanoseconds>(speedCPUTesterEnd - speedCPUTesterBegin).count() << "[ns]" << std::endl;
}

static void printCPUMicroSeconds(){
    std::cout << "CPU = " << std::chrono::duration_cast<std::chrono::microseconds>(speedCPUTesterEnd - speedCPUTesterBegin).count() << "[us]" << std::endl;
}

static void printCPUMilliSeconds(){
    std::cout << "CPU = " << std::chrono::duration_cast<std::chrono::milliseconds>(speedCPUTesterEnd - speedCPUTesterBegin).count() << "[ms]" << std::endl;
}

static void printCPUSeconds(){
    std::cout << "CPU = " << std::chrono::duration_cast<std::chrono::seconds>(speedCPUTesterEnd - speedCPUTesterBegin).count() << "[s]" << std::endl;
}