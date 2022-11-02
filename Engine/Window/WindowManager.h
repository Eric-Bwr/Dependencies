#pragma once

#include "Window.h"

class WindowManager {
public:
    static void init(int versionMajor, int versionMinor);
    static void addWindow(Window* window);
    static void pollEvents();
    static const GLFWvidmode* getVideoMode();
    ~WindowManager();
};