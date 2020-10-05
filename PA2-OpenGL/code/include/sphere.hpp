#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>

// (PA2): Copy from PA1

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
    }

    Sphere(const Vector3f &_center, float _radius, Material *_material) : Object3D(_material) {
        center = _center;
        radius = _radius;
    }


    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

protected:
    Vector3f center;
    float radius;

};


#endif
