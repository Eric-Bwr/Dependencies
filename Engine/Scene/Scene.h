#pragma once

class Scene {
public:
    virtual void init() {}
    virtual void load() {}
    virtual void unload() {}
    virtual void update(double frameDeltaTime) {}
    virtual void render(double frameDeltaTime) {}
    virtual void onKey(int key, int scancode, int action, int mods) {}
    virtual void onChar(unsigned int key) {}
    virtual void onMousePosition(double x, double y) {}
    virtual void onMouseButton(int button, int action, int mods) {}
    virtual void onMouseScroll(double x, double y) {}
    virtual void onResize(bool show, int width, int height) {}
    virtual int getID() = 0;
    virtual ~Scene() = default;
};