//
// Created by sneeuwpop on 23-6-19.
//

#pragma once

#include <glm/glm.hpp>
using namespace glm;


class Camera
{
public:
    Camera();
    ~Camera();

    vec3 position = vec3(0,0,0);
    vec3 up = vec3(0, 1, 0);

    mat4 projection = mat4(1.0); // Todo: it is not clear this is the identity matrix
    mat4 view = mat4(1.0);
    mat4 combined = mat4(1.0); // projection * view

    void update();
};