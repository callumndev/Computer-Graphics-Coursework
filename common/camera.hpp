#pragma once

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <common/maths.hpp>

class Camera
{
public:
    // Constructor
    Camera(const glm::vec3 eye, const glm::vec3 target);

    // Methods
    void calculateMatrices();
    glm::mat4 calculateMVPFromModel(glm::mat4 model);

    // Public camera vecs
    glm::vec3 eyePos;

    // Camera positional offsets
    glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

private:
    // Projection parameters
    float fov = Maths::radians(90.0f);

    // Camera vecs
    glm::vec3 target;

    // Transformation matrices
    glm::mat4 view;
    glm::mat4 projection;
};
