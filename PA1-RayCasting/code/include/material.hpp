#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

// Implement Shade function that computes Phong introduced in class.
class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {
    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    static double clip(double x, double mn = 0.0, double mx = 1.0) {
        return x > mx ? mx : x < mn ? mn : x;
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;
        Vector3f N = hit.getNormal(), L = dirToLight, V = -ray.getDirection();
        Vector3f R = 2 * Vector3f::dot(N, L) * N - L;
        // N.print(), L.print(), V.print(), R.print();
        shaded += lightColor * diffuseColor * clip(Vector3f::dot(N, L));
        shaded += lightColor * specularColor * pow(clip(Vector3f::dot(V, R)), shininess);
        return shaded;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


#endif // MATERIAL_H
