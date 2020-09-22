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

    static void flip_v(int &x, int &y, int h) {
        y = h - 1 - y;
    }
    static void flip_xy(int &x, int &y) {
        std::swap(x, y);
    }
    static void trans(int &x, int &y, int f, int w, int h)
    {
        if (f & 1) flip_v(x, y, h);
        if ((f >> 1) & 1) flip_xy(x, y);
    }
    static void itrans(int &x, int &y, int f, int w, int h)
    {
        if ((f >> 1) & 1) flip_xy(x, y);
        if (f & 1) flip_v(x, y, h);
    }

    static void set_trans(int x, int y, int f, const Vector3f &color, Image &img)
    {
        itrans(x, y, f, img.Width(), img.Height());
        // printf("@");
        img.SetPixel(x, y, color);
    }

    // draw a line from (x0, y0) to (x1, y1) on trans(img, f)
    // requirement: x0 <= x1, y0 <= y1, dy <= dx
    static void _draw(int x0, int y0, int x1, int y1, int f, const Vector3f &color, Image &img)
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

    static void set_sym(int x, int y, int x0, int y0, const Vector3f &color, Image &img)
    {
        int xs[] = {x, x, -x, -x, y, y, -y, -y};
        int ys[] = {y, -y, y, -y, x, -x, x, -x};
        for (int i = 0; i < 8; i++)
            img.SetPixel(x0 + xs[i], y0 + ys[i], color);
    }
    static void _draw(int x0, int y0, int r, const Vector3f &color, Image &img)
    {
        int x = 0, y = r;
        double d = 1.25 - r;
        set_sym(x, y, x0, y0, color, img);
        while (x <= y) {
            if (d < 0)
                d += 2 * x + 3;
            else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
            set_sym(x, y, x0, y0, color, img);
        }
    }
    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
        _draw(cx, cy, radius, color, img);
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;

    static void _draw(int x0, int y0, const Vector3f &color, Image &img) {
        static int dx[] = {1, -1, 0, 0};
        static int dy[] = {0, 0, 1, -1};
        Vector3f old_color = img.GetPixel(x0, y0);
        if (old_color == color)
            return ;
        int w = img.Width(), h = img.Height();
        std::queue<std::pair<int, int> > Q;
        Q.push(std::make_pair(x0, y0));
        while (!Q.empty()) {
            int x = Q.front().first, y = Q.front().second;
            Q.pop();
            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i], ny = y + dy[i];
                if (0 <= nx && nx < w && 0 <= ny && ny < h)
                    if (img.GetPixel(x + dx[i], y + dy[i]) == old_color) {
                        img.SetPixel(x + dx[i], y + dy[i], color);
                        Q.push(std::make_pair(x + dx[i], y + dy[i]));
                    }
            }
        }
    }

    void draw(Image &img) override {
        // TODO: Flood fill
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
        _draw(cx, cy, color, img);
    }
};