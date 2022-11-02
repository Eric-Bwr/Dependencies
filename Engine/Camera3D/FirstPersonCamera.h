#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera {
public:
    void init(double x, double y, double z, float yaw, float pitch);
    void update();
    void moveMouse(double xPos, double yPos);
private:
    Vec3d front, up, right, worldUp;
};