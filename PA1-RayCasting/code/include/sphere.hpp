#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
    }

    Sphere(const Vector3f &center, float radius, Material *_material) : Object3D(_material) {
        o = center;
        r = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        Vector3f a = ray.getOrigin(), d = ray.getDirection();
        // cerr << a[0] << ' ' << a[1] << ' ' << a[2] << ' ' << d[0] << ' ' << d[1] << ' '<< d[2] << endl;
        Vector3f ao = o - a;
        double l = Vector3f::dot(ao, d), ao2 = Vector3f::dot(ao, ao);
        // cerr << l << ' ' << ao2 << endl;
        double h2 = ao2 - l * l;
        // cerr << h2 << ' ' << r * r << endl;
        if (h2 > r * r) {
            return false;
        }
        double ll = l - sqrt(r * r - h2);
        Vector3f point = a + ll * d, normal = point - o;
        if (hit.getT() > ll) {
            hit.set(ll, material, normal);
        }
        return true;
    }

protected:
    Vector3f o;
    double r;
};


#endif
