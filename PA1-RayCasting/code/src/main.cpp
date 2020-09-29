#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <ctime>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

Vector3f clip_vec(const Vector3f &a, double mn = 0.0, double mx = 1.0) {
    Vector3f ret = a;
    for (int i = 0; i < 3; i++)
        ret[i] = a[i] > mx ? mx : a[i] < mn ? mn : a[i];
    return ret;
}

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    // cout << "Hello! Computer Graphics!" << endl;
    
    cout << "Rendering started." << endl;
    std::clock_t c_start = std::clock();

    SceneParser *sceneParser = new SceneParser(inputFile.c_str());
    Camera *camera = sceneParser->getCamera();
    int w = camera->getWidth(), h = camera->getHeight();
    Image *image = new Image(w, h);

    for (int x = 0; x < camera->getWidth(); x++)
        for (int y = 0; y < camera->getHeight(); y++) {
            // if (x != 100 || y != 66) continue;
            Ray camRay = camera->generateRay(Vector2f(x, y));
            Group *baseGroup = sceneParser->getGroup();
            Hit hit;
            bool isIntersect = baseGroup->intersect(camRay, hit, 0);
            if (isIntersect) {
                Vector3f finalColor = Vector3f::ZERO;
                for (int i = 0; i < sceneParser->getNumLights(); i++) {
                    Light *light = sceneParser->getLight(i);
                    Vector3f L, lightColor;
                    light->getIllumination(camRay.pointAtParameter(hit.getT()), L, lightColor);
                    finalColor += hit.getMaterial()->Shade(camRay, hit, L, lightColor);
                }
                image->SetPixel(x, y, clip_vec(finalColor));
                // image->SetPixel(x, y, 1.0 - sceneParser->getBackgroundColor());
            }
            else {
                image->SetPixel(x, y, sceneParser->getBackgroundColor());
            }
        }
    EXIT:;
    image->SaveImage(outputFile.c_str());
    
    std::clock_t c_end = std::clock();
    cout << "Rendering finished. ";
    cout << "(CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC <<"ms)" << endl;
    return 0;
}

