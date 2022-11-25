#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
    void initWindow(const char* title, int width, int height, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
    void hint(int hint, int value);
    void setIcon(const char* icon) const;
    void setPos(int x, int y) const;
    void setMouse(double xpos, double ypos) const;
    void center(int xOffset = 0, int yOffset = 0) const;
    void swapInterval(int swapInterval = 1) const;
    void context() const;
    void swapBuffers() const;
    ~Window();

    virtual void keyCallback(int key, int scancode, int action, int mods) = 0;
    virtual void charCallback(unsigned int key) = 0;
    virtual void mousePositionCallback(double x, double y) = 0;
    virtual void mouseButtonCallback(int button, int action, int mods) = 0;
    virtual void scrollCallback(double x, double y) = 0;
    virtual void resizeCallback(int width, int height) = 0;
    virtual void closeCallback() = 0;

    GLFWwindow* window;
    int width, height;
    bool alive = true;
};