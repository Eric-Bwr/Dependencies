#include <OpenGL/OpenGL.h>
#include "Window.h"
#include "WindowManager.h"
#include <Logger/Logger.h>
#include <OpenGL/GLUtil/STB.h>

static bool openGLLoaded = false;

void Window::initWindow(const char *title, int width, int height, GLFWmonitor* monitor, GLFWwindow* share) {
    this->width = width;
    this->height = height;
    logInfo("Creating GLFW Window");
    window = glfwCreateWindow(width, height, title, monitor, share);
    if(!window){
        logError("Failed to create Window");
        glfwDestroyWindow(window);
    }
    glfwMakeContextCurrent(window);
    if(openGLLoaded)
        return;
    openGLLoaded = true;
    if(!gladLoadGL(glfwGetProcAddress)){
        logError("Failed to load OpenGL");
        glfwDestroyWindow(window);
    }
}

void Window::setIcon(const char * icon) const {
    logInfo("Loading Window Icon: ", icon);
    GLFWimage images[1];
    images[0].pixels = stbi_load(icon, &images[0].width, &images[0].height, nullptr, 0);
    if (images[0].pixels != nullptr) {
            glfwSetWindowIcon(window, 1, images);
        } else {
        logError("Failed to load Window Icon.png");
    }
    free(images[0].pixels);
}

void Window::setPos(int x, int y) const {
    glfwSetWindowPos(window, x, y);
}

void Window::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    glfwSetWindowSize(window, width, height);
}

void Window::center(int xOffset, int yOffset) const {
    glfwSetWindowPos(window, (WindowManager::getVideoMode()->width / 2) - (width / 2) + xOffset, (WindowManager::getVideoMode()->height / 2) - (height / 2) + yOffset);
}

void Window::hint(int hint, int value) {
    glfwWindowHint(hint, value);
}

void Window::setMouse(double xpos, double ypos) const {
    glfwSetCursorPos(window, xpos, ypos);
}

void Window::swapInterval(int swapInterval) const {
    glfwSwapInterval(swapInterval);
}

void Window::context() const {
    if(glfwGetCurrentContext() != window)
        glfwMakeContextCurrent(window);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

Window::~Window() {
    alive = false;
    glfwDestroyWindow(window);
}