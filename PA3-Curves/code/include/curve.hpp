#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>
#include <iostream>

#include <algorithm>

// (PA3): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
    CurvePoint normalized() {
        return (CurvePoint){V, T.normalized()};
    }
};


class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};

enum CurveType {
    BSPINE_CURVE, BEZIER_CURVE,
};
class Bernstein {
private:
    int n, k;
    std::vector<Vector3f> controls;
    std::vector<double> knot;
    static constexpr double EPS = 1e-12;
public:
    Bernstein(int n, int k, CurveType type = BSPINE_CURVE) {
        // std::cerr << n << ' ' << k << ' ' << "type: " << type << std::endl;
        knot.resize(n + k + 2);
        this->n = n, this->k = k;
        if (type == BSPINE_CURVE) {
            for (int i = 0; i <= n + k + 1; i++)
                knot[i] = (double)i / (n + k + 1);
        }
        else if (type == BEZIER_CURVE) {
            assert(n == k);
            for (int i = 0; i <= n; i++)
                knot[i] = 0, knot[2 * n + 1 - i] = 1;
        }
    }
    double get_l() {
        return knot[k] + EPS;
    }
    double get_r() {
        return knot[n + 1] - EPS;
    }
    CurvePoint evaluate(double x, const std::vector<Vector3f> &points) {
        // std::cerr << n << ' ' << k << ' ' << knot.size() << ' ' << x << std::endl;
        assert(0 <= x && x <= 1);
        CurvePoint ret = {};
        std::vector<double> v(n + k + 2, 0);
        for (int i = 0; i < n + k + 1; i++)
            v[i] = (knot[i] <= x && x < knot[i + 1]);
        for (int p = 1; p <= k; p++) {
            for (int i = 0; i <= n + k - p; i++) {
                double nv = 0.0;
                if (knot[i + p] > knot[i])
                    nv += (x - knot[i]) / (knot[i + p] - knot[i]) * v[i];
                if (knot[i + p + 1] > knot[i + 1])
                    nv += (knot[i + p + 1] - x) / (knot[i + p + 1] - knot[i + 1]) * v[i + 1];
                v[i] = nv;
            }
            if (p == k - 1) {
                ret.T = 0;
                for (int i = 0; i <= n; i++) {
                    double vp = 0.0;
                    if (knot[i + k] > knot[i])
                        vp += k * v[i] / (knot[i + k] - knot[i]);
                    if (knot[i + k + 1] > knot[i + 1])
                        vp -= k * v[i + 1] / (knot[i + k + 1] - knot[i + 1]);
                    ret.T += points[i] * vp;
                }
                // std::cerr << sum << std::endl;
            }
        }
        ret.V = 0;
        for (int i = 0; i <= n; i++)
            ret.V += points[i] * v[i];
        // double sum = 0;
        // for (int i = 0; i <= n; i++)
        //     sum += v[i];
        // std::cerr << sum << std::endl;
        return ret.normalized();
    }
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // (PA3): fill in data vector
        int n = controls.size() - 1, k = n; 
        resolution *= n;
        Bernstein B(n, k, BEZIER_CURVE);
        double l = B.get_l(), r = B.get_r();
        for (int i = 0; i <= resolution; i++) {
            double x = (r - l) * i / resolution + l;
            data.push_back(B.evaluate(x, controls));
            // std::cerr << data.back().V << std::endl;
        }
    }

protected:

};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // (PA3): fill in data vector
        int n = controls.size() - 1, k = 3; 
        resolution *= n;
        Bernstein B(n, k, BSPINE_CURVE);
        double l = B.get_l(), r = B.get_r();
        for (int i = 0; i <= resolution; i++) {
            double x = (r - l) * i / resolution + l;
            data.push_back(B.evaluate(x, controls));
        }
    }

protected:

};

#endif // CURVE_HPP
