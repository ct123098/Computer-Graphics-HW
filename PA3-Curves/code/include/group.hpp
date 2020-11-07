#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// (PA2): Implement Group - copy from PA1
class Group : public Object3D {

public:

    Group() {

    }

    explicit Group (int num_objects) {
        vec.resize(num_objects);
    }

    ~Group() override {

    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    void drawGL() override {
        for (int i = 0; i < vec.size(); i++)
            if (vec[i] != nullptr) {
                vec[i]->drawGL();
            }
    }

    void addObject(int index, Object3D *obj) {
        vec[index] = obj;
    }

    int getGroupSize() {
        return vec.size();
    }

private:
    std::vector<Object3D*> vec;

};

#endif
	
