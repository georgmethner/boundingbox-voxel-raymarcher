//
// Created by georg on 5/20/2024.
//

#include <stdexcept>
#include "Object.hpp"
#include "matrix_transform.hpp"


Object::Object(std::string name) {
    Object::name = std::move(name);

    object_matrix = glm::mat4(1.0f);
}

GLuint Object::Load(const char *path, unsigned int index) {
    MV_Model mv_model;
    bool result = mv_model.LoadModel(path);
    if (!result) {
        throw (std::runtime_error("Failed to load model"));
    }

    //CREATE VOXELS

    char voxels[mv_model.size_x][mv_model.size_y][mv_model.size_z];

    for (int x = 0; x < mv_model.size_x; x++)
        for (int y = 0; y < mv_model.size_y; y++)
            for (int z = 0; z < mv_model.size_z; z++)
                voxels[x][y][z] = 0;

    for (int i = 0; i < mv_model.num_voxels; i++) {
        MV_Voxel voxel = mv_model.voxels[i];
        voxels[voxel.x][voxel.z][voxel.y] = voxel.colorIndex; // or any other value you want to assign
    }


    //SET TEXTURE

    GLuint temp_texture;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &temp_texture);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, temp_texture);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, mv_model.size_x, mv_model.size_y, mv_model.size_z, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, voxels);

    glBindTexture(GL_TEXTURE_3D, 0);

    //CONSTRUCT BOX MESH

    glm::vec3 scale = glm::vec3(mv_model.size_x / 16.0, mv_model.size_y / 16.0, mv_model.size_z / 16.0);
    object_matrix = glm::scale(object_matrix, scale);


    Object::texture = temp_texture;
    return temp_texture;
}




