#include "Controller.h"

Vec2d Controller::computeVelocity(double strafe, double forward, double slipperiness) {
    double speed = std::sqrt(strafe * strafe + forward * forward);
    if (speed < 0.01)
        return Vec2d(0.0);
    speed = slipperiness / std::max(speed, 1.0);

    strafe = strafe * speed;
    forward = forward * speed;

    double yawYComponent = std::sin((yaw - 90) * RADIANS);
    double yawXComponent = std::cos((yaw - 90) * RADIANS);

    return Vec2d{strafe * yawXComponent - forward * yawYComponent, forward * yawXComponent + strafe * yawYComponent};
}

Vec2d Controller::computeMovement() {
    if (!(shouldMoveForward && shouldMoveBackward)) {
        if (!shouldMoveForward && !shouldMoveBackward)
            moveForward = 0.0;
        if (shouldMoveForward)
            moveForward = 1.0;
        if (shouldMoveBackward)
            moveForward = -1.0;
    } else
        moveForward = 0.0;
    if (!(shouldMoveLeft && shouldMoveRight)) {
        if (!shouldMoveLeft && !shouldMoveRight)
            moveStrafe = 0.0;
        if (shouldMoveLeft)
            moveStrafe = 1.0;
        if (shouldMoveRight)
            moveStrafe = -1.0;
    } else
        moveStrafe = 0.0;

    return computeVelocity(moveStrafe, moveForward, 0.06);
}