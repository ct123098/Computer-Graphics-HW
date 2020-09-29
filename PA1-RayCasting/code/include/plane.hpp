#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        n = normal;
        o = d * n;
    }

    ~Plane() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        const double EPS = 1e-8;
        Vector3f a = ray.getOrigin(), d = ray.getDirection();
        if (abs(Vector3f::dot(d, n)) < EPS)
            return false;
        Vector3f ao = o - a;
        double h = Vector3f::dot(ao, n);
        Vector3f nn = n * (h >= 0 ? 1 : -1);
        h = abs(h);
        double cost = Vector3f::dot(d, nn);
        double l = h / cost;
        if (l < tmin)
            return false;
        Vector3f point = a + l * d, normal = -nn;
        if (hit.getT() > l) {
            hit.set(l, material, normal);
        }
        return true;
    }

protected:
    Vector3f o, n;

};

#endif //PLANE_H
		

