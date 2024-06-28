//
// Created by georg on 5/15/2024.
//

#ifndef VOXEL_RAYMARCHER_OBJECTHANDLER_HPP
#define VOXEL_RAYMARCHER_OBJECTHANDLER_HPP

#include "vec3.hpp"
#include "glad.h"
#include "Object.hpp"
#include <string>
#include <vector>


class ObjectHandler {

public:
    ObjectHandler();
    static void AddObject(const Object& object);
    static std::vector<Object> objects;
    static std::vector<glm::mat4> object_matrices;
    static unsigned int object_count;
};


#endif //VOXEL_RAYMARCHER_OBJECTHANDLER_HPP
