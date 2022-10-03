#include "SkyBox.h"

void SkyBox::init(const char *path, const char* ending) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    texture = new Texture(path, ending, 0, false);
    texture->minLinear();
    texture->magLinear();
    texture->clampEdge();
}

void SkyBox::update(float skyBoxRotation, float rotationX, float rotationY, float rotationZ){
    this->skyBoxRotation = skyBoxRotation;
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->rotationZ = rotationZ;
}

SkyBox::~SkyBox() {
    delete texture;
}