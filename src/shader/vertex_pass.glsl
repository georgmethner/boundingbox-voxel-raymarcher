#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;

layout (binding = 0) buffer object_matrices
{
    mat4 object_matrix[];
};


uniform mat4 proj;
uniform mat4 view;

uniform uint object_index;

const float GRID = 16;


out vec2 UVs;

void main()
{

    gl_Position = inverse(proj) * inverse(view) * object_matrix[object_index] * vec4(pos, 1.0);
    UVs = uvs;
}