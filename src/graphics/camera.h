#pragma once

#include <vector>
#include <ramiel/math.h>

namespace ramiel {
    
    const Vec2u& getRes();
    void setRes(Vec2u size);
    size_t getBufferSize();

    typedef std::vector<Vec3f>::iterator ColorBufferIterator;
    typedef std::vector<float>::iterator DepthBufferIterator;
    ColorBufferIterator getColorBuffer();
    DepthBufferIterator getDepthBuffer();

    const Vec3f& getPos();
    const Vec3f& getRot();

    void setPos(const Vec3f& pos);
    void setRot(const Vec3f& rot);

    float getFov();
    float getZ0();
    float getZ1();

    void setFov(float fov);
    void setZ0(float z0);
    void setZ1(float z1);

    Vec3f getCameraCoord(const Vec3f& in);
    Vec2f getScreenCoord(const Vec3f& in);

    const Vec3f& getBackgroundColor();
    void setBackgroundColor(const Vec3f& color);

    void resetBuffers();
    void clampColorBuffer();

    void getFrameDEC(int* frame);
    void getFrameRGB(uint8_t* frame);

}
