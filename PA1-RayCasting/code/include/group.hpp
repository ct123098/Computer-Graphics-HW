#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO: Implement Group - add data structure to store a list of Object*
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
        bool ret;
        for (int i = 0; i < vec.size(); i++)
            if (vec[i] != nullptr) {
                ret |= vec[i]->intersect(r, h, tmin);
            }
        return ret;
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
	
