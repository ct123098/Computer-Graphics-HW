#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        n = normal.normalized();
        o = d * n;
    }

    ~Plane() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        const double EPS = 1e-8;
        Vector3f a = ray.getOrigin(), d = ray.getDirection();
        double C = Vector3f::dot(n, d);
        if (abs(C) < EPS)
            return false;
        double A = Vector3f::dot(n, o), B = Vector3f::dot(n, a);
        double l = (A - B) / C;
        if (l < tmin)
            return false;
        Vector3f point = a + l * d, normal = -n * (C >= 0 ? 1 : -1);
        if (hit.getT() > l) {
            hit.set(l, material, normal);
        }
        return true;
    }

protected:
    Vector3f o, n;

};

#endif //PLANE_H
		

