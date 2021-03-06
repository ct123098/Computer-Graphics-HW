#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        double t = (2 * tan(angle / 2)), f = std::min(width, height) / t;
        // fx = width / t, fy = height / t; // make both edge fit the angle
        fx = f, fy = f; // make the shorter edge fit the angle
        cx = width / 2.0, cy = height / 2.0;
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f d0((point.x() - cx) / fx, (point.y() - cy) / fy, -1.0);
        Matrix3f R(horizontal, up, -direction);
        Ray ret(center, (R * d0).normalized());
        return ret;
    }

protected:
    double cx, cy;
    double fx, fy;

};

#endif //CAMERA_H
