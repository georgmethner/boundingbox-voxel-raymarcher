//
// Created by georg on 5/20/2024.
//

#ifndef VOXEL_RAYMARCHER_OBJECT_HPP
#define VOXEL_RAYMARCHER_OBJECT_HPP

#include "vec3.hpp"
#include "glad.h"
#include "MagicaVoxelModel.hpp"
#include "mat4x4.hpp"
#include <string>


class Object {
public:
    Object() = default;
    Object(std::string name);
    std::string name;
    glm::mat4 object_matrix;
    GLuint Load(const char* path, unsigned int index);
    GLuint texture;

};


#endif //VOXEL_RAYMARCHER_OBJECT_HPP
