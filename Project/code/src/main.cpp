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
#include <thread>


using namespace std;

Vector3f clip_vec(const Vector3f &a, double mn = 0.0, double mx = 1.0) {
    Vector3f ret = a;
    for (int i = 0; i < 3; i++)
        ret[i] = a[i] > mx ? mx : a[i] < mn ? mn : a[i];
    return ret;
}

struct ArgumentsConfig {
    string inputFile;
    string outputFile;
};
ArgumentsConfig parse_args(int argc, char *argv[]) {
    ArgumentsConfig args;
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        exit(1);
    }
    args.inputFile = argv[1];
    args.outputFile = argv[2]; // only bmp is allowed.
    return args;
}

Vector3f calc_pixel(const Ray &ray, const SceneParser *sceneParser) {
    // std::cerr << "!" << endl;
    // std::cerr << ray.getDirection() << endl;
    Group *baseGroup = sceneParser->getGroup();
    Hit hit;
    bool isIntersect = baseGroup->intersect(ray, hit, 0);
    if (isIntersect) {
        Vector3f finalColor = Vector3f::ZERO;
        for (int i = 0; i < sceneParser->getNumLights(); i++) {
            Light *light = sceneParser->getLight(i);
            Vector3f L, lightColor;
            light->getIllumination(ray.pointAtParameter(hit.getT()), L, lightColor);
            finalColor += hit.getMaterial()->Shade(ray, hit, L, lightColor);
        }
        return clip_vec(finalColor);
    }
    else {
        return sceneParser->getBackgroundColor();
    }
}

struct GlobalInfo {
    SceneParser *sceneParser;
    Camera *camera;
};
typedef pair<int, int> pii;

// void worker_func() {
//     std::cerr << "!" << endl;
// }
void worker_func(const GlobalInfo *info, std::vector<pii> points, Image *image) {
    SceneParser *sceneParser = info->sceneParser;
    Camera *camera = info->camera;
    for (auto it = points.begin(); it != points.end(); it++) {
        int x = it->first, y = it->second;
        Ray camRay = camera->generateRay(Vector2f(x, y));
        Vector3f color = calc_pixel(camRay, sceneParser);
        image->SetPixel(x, y, color);
    }
}

int main(int argc, char *argv[]) {
    ArgumentsConfig args = parse_args(argc, argv);
    
    // Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    // cout << "Hello! Computer Graphics!" << endl;
    
    cout << "Rendering started." << endl;
    std::clock_t c_start = std::clock();

    SceneParser *sceneParser = new SceneParser(args.inputFile.c_str());
    Camera *camera = sceneParser->getCamera();
    int W = camera->getWidth(), H = camera->getHeight();
    Image *image = new Image(W, H);

    int n_workers = 8;
    const int MAX_WORKERS = 128;
    GlobalInfo info = {sceneParser, camera};
    std::vector<pii> points[MAX_WORKERS];
    for (int x = 0, t = 0; x < W; x++)
        for (int y = 0; y < H; y++, t = (t + 1) % n_workers)
            points[t].push_back(make_pair(x, y));
    std::thread t[MAX_WORKERS];
    for (int i = 0; i < n_workers; i++) {
        t[i] = std::thread(worker_func, &info, points[i], image);
        // t[i] = std::thread(worker_func);
    }
    for (int i = 0; i < n_workers; i++) {
        t[i].join();
    }

    // for (int x = 0; x < W; x++)
    //     for (int y = 0; y < H; y++) {
    //         Ray camRay = camera->generateRay(Vector2f(x, y));
    //         Vector3f color = calc_pixel(camRay, sceneParser);
    //         image->SetPixel(x, y, color);
    //     }

    image->SaveImage(args.outputFile.c_str());
    
    delete sceneParser;
    delete image;
    
    std::clock_t c_end = std::clock();
    cout << "Rendering finished. ";
    cout << "(CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC <<"ms)" << endl;
    return 0;
}

