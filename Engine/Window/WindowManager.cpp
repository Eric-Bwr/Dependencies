#include "WindowManager.h"
#include <Logger/Logger.h>
#include <map>

static std::map<GLFWwindow*, Window*> windows;

static const GLFWvidmode* videoMode;

void WindowManager::init(int versionMajor, int versionMinor) {
    logInfo("Initializing GLFW");
    if(!glfwInit()){
        logError("Failed to init GLFW");
        exit(-1);
    }
    videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
}

static void innerKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    windows[window]->keyCallback(key, scancode, action, mods);
}

static void innerCharCallback(GLFWwindow *window, unsigned int key) {
    windows[window]->charCallback(key);
}

static void innerMousePositionCallback(GLFWwindow *window, double x, double y) {
    windows[window]->mousePositionCallback(x, y);
}

static void innerMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    windows[window]->mouseButtonCallback(button, action, mods);
}

static void innerScrollCallback(GLFWwindow *window, double x, double y) {
    windows[window]->scrollCallback(x, y);
}

static void innerResizeCallback(GLFWwindow *window, int width, int height) {
    windows[window]->resizeCallback(width, height);
}

static void innerCloseCallback(GLFWwindow *window) {
    windows[window]->alive = false;
    windows[window]->closeCallback();
}

void WindowManager::addWindow(Window* window) {
    windows.insert({window->window, window});
    glfwSetKeyCallback(window->window, innerKeyCallback);
    glfwSetCharCallback(window->window, innerCharCallback);
    glfwSetCursorPosCallback(window->window, innerMousePositionCallback);
    glfwSetMouseButtonCallback(window->window, innerMouseButtonCallback);
    glfwSetScrollCallback(window->window, innerScrollCallback);
    glfwSetFramebufferSizeCallback(window->window, innerResizeCallback);
    glfwSetWindowCloseCallback(window->window, innerCloseCallback);
}

void WindowManager::pollEvents() {
    glfwPollEvents();
}

const GLFWvidmode *WindowManager::getVideoMode() {
    return videoMode;
}

WindowManager::~WindowManager() {
    glfwTerminate();
}