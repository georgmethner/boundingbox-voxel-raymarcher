//
// Created by georg on 5/15/2024.
//

#include <iostream>
#include "ObjectHandler.hpp"
#include "matrix_transform.hpp"

#include <random>
#include <ctime>


unsigned int ObjectHandler::object_count = 0;
std::vector<Object> ObjectHandler::objects;
std::vector<glm::mat4> ObjectHandler::object_matrices;



float randomFloat(float min, float max) {
    static std::mt19937 generator(std::time(0)); // Mersenne twister generator
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

void ObjectHandler::AddObject(const Object &object) {
    ObjectHandler::object_count++;

    ObjectHandler::objects.push_back(object);
    ObjectHandler::object_matrices.push_back(object.object_matrix);
}

ObjectHandler::ObjectHandler() {

    Object object = Object("test");
    //object.object_matrix = glm::translate(object.object_matrix, glm::vec3(10.0f, 0.0f, 0.0f));
    //object.Load("../src/models/monu10.vox", 0);
    //ObjectHandler::AddObject(object);


    for (int x = 0; x < 20; x++) {
        for (int z = 0; z < 20; z++) {
            object = Object("test");
            object.object_matrix = glm::translate(object.object_matrix, glm::vec3(x * 5.0f, 0.0f, z * 5.0f));

            // Generate a random angle between 0 and 360 degrees
            float angle = randomFloat(0.0f, 360.0f);
            glm::vec3 axis = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));

            // Rotate the object using the random angle
            object.object_matrix = glm::rotate(object.object_matrix, glm::radians(angle), axis);

            object.Load("../src/models/castle.vox", x * 20 + z);
            ObjectHandler::AddObject(object);
        }
    }


    //CREATE OBJECT_MATRICES BUFFER
    GLuint object_matrices_buffer;

    glGenBuffers(1, &object_matrices_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, object_matrices_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, object_matrices.size() * sizeof(glm::mat4), &object_matrices[0][0][0], GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, object_matrices_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}


