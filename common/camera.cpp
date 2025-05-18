#include <common/camera.hpp>

// Constructor
Camera::Camera(const glm::vec3 eye, const glm::vec3 _target)
{
    // Populate inital values
    eyePos = eye;
    target = _target;
}

// Calculates the view matrix
// Calculates the projection matrix
void Camera::calculateMatrices()
{
    // View matrix
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(eyePos, eyePos + cameraFront, worldUp);

    // Projection matrix
    float aspectVal = 1920.0f / 1080.0f;
    projection = glm::perspective(fov, aspectVal, 0.2f, 100.0f);
}

// Retrusn the MVP for the specified model
glm::mat4 Camera::calculateMVPFromModel(glm::mat4 model)
{
    return projection * view * model;
}
