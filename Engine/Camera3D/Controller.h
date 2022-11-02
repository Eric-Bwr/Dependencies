#pragma once

#include <Math/Matrix.h>

class Controller {
public:
    Vec2d computeVelocity(double strafe, double forward, double slipperiness);
    Vec2d computeMovement();
    Vec3d position;
    double yaw, pitch;
    double moveForward = 0.0;
    double moveStrafe = 0.0;
    bool shouldMoveForward = false, shouldMoveBackward = false, shouldMoveRight = false, shouldMoveLeft = false, shouldMoveUp = false, shouldMoveDown = false;
};