#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <cstdlib>

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;

    void flip_v(int &x, int &y, int h) {
        y = h - 1 - y;
    }
    void flip_xy(int &x, int &y) {
        std::swap(x, y);
    }
    void trans(int &x, int &y, int f, int w, int h)
    {
        if (f & 1) flip_v(x, y, h);
        if ((f >> 1) & 1) flip_xy(x, y);
    }
    void itrans(int &x, int &y, int f, int w, int h)
    {
        if ((f >> 1) & 1) flip_xy(x, y);
        if (f & 1) flip_v(x, y, h);
    }

    void set_trans(int x, int y, int f, const Vector3f &color, Image &img)
    {
        itrans(x, y, f, img.Width(), img.Height());
        // printf("@");
        img.SetPixel(x, y, color);
    }

    // draw a line from (x0, y0) to (x1, y1) on trans(img, f)
    // requirement: x0 <= x1, y0 <= y1, dy <= dx
    void _draw(int x0, int y0, int x1, int y1, int f, const Vector3f &color, Image &img)
    {
        int x = x0, y = y0, dx = x1 - x0, dy = y1 - y0, e = -dx;
        // printf("@ x0 = %d, y0 = %d, x1 = %d, y1 = %d\n", x0, y0, x1, y1);
        for (int i = 0; i <= dx; i++) {
            set_trans(x, y, f, color, img);
            x++; e += 2 * dy;
            while (e >= 0) {
                y++; e -= 2 * dx;
            }
        }
    }

    void draw(Image &img) override {
        // TODO: Implement Bresenham Algorithm
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
                color.x(), color.y(), color.z());
        if (xA > xB) {
            std::swap(xA, xB), std::swap(yA, yB);
        }
        int w = img.Width(), h = img.Height();
        int dx = xB - xA, dy = yB - yA, f = 0;
        // transfer parameter f = ab --> a:0/1 flip along x=y, b:0/1 flip along y=(h-1)/2
        // transfer rule
        if (dy > 0 && dy > dx)
            f = 2;
        else if(dy >= 0 && dy <= dx)
            f = 0;
        else if(dy < 0 && -dy <= dx)
            f = 1;
        else if (dy < 0 && -dy > dx)
            f = 3;
        else
            assert(false && "This statement should not be reach.");
        // printf("@ dx = %d, dy = %d, f = %d\n", dx, dy, f);
        trans(xA, yA, f, w, h);
        trans(xB, yB, f, w, h);
        _draw(xA, yA, xB, yB, f, color, img);
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;

    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }
};