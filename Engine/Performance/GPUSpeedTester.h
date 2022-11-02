#pragma once

#include <iostream>
#include <OpenGL/OpenGL.h>

static GLuint64 speedGPUTesterBegin, speedGPUTesterEnd;
static unsigned int queryID[2];

static void beginGPUSpeedTest(){
    glGenQueries(2, queryID);
    glQueryCounter(queryID[0], GL_TIMESTAMP);
}

static void endGPUSpeedTest(){
    glQueryCounter(queryID[1], GL_TIMESTAMP);
    int stopTimerAvailable = 0;
    while (!stopTimerAvailable)
        glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
    glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &speedGPUTesterBegin);
    glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &speedGPUTesterEnd);
    glDeleteQueries(2, queryID);
}

static void printGPUNanoSeconds(){
    std::cout << "GPU = " << (speedGPUTesterEnd - speedGPUTesterBegin) << "[ns]" << std::endl;
}

static void printGPUMicroSeconds(){
    std::cout << "GPU = " << (speedGPUTesterEnd - speedGPUTesterBegin) / 1000.0 << "[us]" << std::endl;
}

static void printGPUMilliSeconds(){
    std::cout << "GPU = " << (speedGPUTesterEnd - speedGPUTesterBegin) / 1000000.0 << "[ms]" << std::endl;
}

static void printGPUSeconds(){
    std::cout << "GPU = " << (speedGPUTesterEnd - speedGPUTesterBegin) / 1000000000.0 << "[s]" << std::endl;
}