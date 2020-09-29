#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		o = a;
		v1 = b - a;
		v2 = c - a;
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        const double EPS = 1e-8;
        Vector3f a = ray.getOrigin(), d = ray.getDirection();
		Vector3f n = Vector3f::cross(v1, v2).normalized();
        double C = Vector3f::dot(n, d);
        if (fabs(C) < EPS)
            return false;
        double A = Vector3f::dot(n, o), B = Vector3f::dot(n, a);
        double l = (A - B) / C;
        if (l < tmin)
            return false;
        Vector3f point = a + l * d, normal = -n * (C >= 0 ? 1 : -1);

		Vector3f b = o + v1, c = o + v2;
		double Sab = Vector3f::dot(Vector3f::cross(o - point, b - point), n);
		double Sbc = Vector3f::dot(Vector3f::cross(b - point, c - point), n);
		double Sca = Vector3f::dot(Vector3f::cross(c - point, o - point), n);
		if (Sab < 0 || Sbc < 0 || Sca < 0)
			return false;

        if (hit.getT() > l) {
            hit.set(l, material, normal);
        }
        return true;
	}
	Vector3f normal;
	Vector3f vertices[3];
protected:
	Vector3f o, v1, v2;
};

#endif //TRIANGLE_H
