#pragma once

#include <vector>

class Scene;

class SceneManager {
public:
    void setCurrent(int id);
    void setCurrent(Scene* scene);
    void add(Scene* scene);
    Scene* getScene(int id);
    void update(double frameDeltaTime);
    void render(double frameDeltaTime);
    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
    void onResize(bool show, int width, int height);
    ~SceneManager();

    std::vector<Scene*> scenes;
    Scene* currentScene;
};