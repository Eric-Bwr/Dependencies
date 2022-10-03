#include "FirstPersonCamera.h"

void FirstPersonCamera::init(double x, double y, double z, float yaw, float pitch) {
    position = {x, y, z};
    this->yaw = yaw;
    this->pitch = pitch;
    viewMatrix.identity();
    worldUp.y = 1;
    front.z = -1;
    update();
}

void FirstPersonCamera::update() {
    front.x = std::cos(yaw * RADIANS) * std::cos(pitch * RADIANS);
    front.y = std::sin(pitch * RADIANS);
    front.z = std::sin(yaw * RADIANS) * std::cos(pitch * RADIANS);
    front.norm();
    right = front.cross(worldUp).norm();
    up = (right.cross(front)).norm();
    viewMatrix.lookAtO(position, front, up);
}

void FirstPersonCamera::moveMouse(double xPos, double yPos) {
    if (!hasLastPos) {
        lastPosX = xPos;
        lastPosY = (yPos * -1);
        hasLastPos = true;
    }
    xOffset = (lastPosX - xPos) * -1;
    yOffset = (yPos * -1) - lastPosY;
    lastPosX = xPos;
    lastPosY = (yPos * -1);

    xOffset *= mouseSpeed;
    yOffset *= mouseSpeed;

    yaw += xOffset;
    pitch += yOffset;
    if (pitch >= pitchMax)
        pitch = pitchMax;
    if (pitch <= pitchMin)
        pitch = pitchMin;
}
