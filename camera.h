#ifndef CAMERAH
#define CAMERAH

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


struct Camera
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;
};

void UpdateCamera(Camera* cam)
{
     cam->front.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
     cam->front.y = sin(glm::radians(cam->pitch));
     cam->front.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
     cam->front = glm::normalize(cam->front);
     cam->right = glm::normalize(glm::cross(cam->front, cam->worldUp));
     cam->up = glm::normalize(glm::cross(cam->right, cam->front));
}

void InitializeCamera(Camera* cam,
                      glm::vec3 startPos,
                      glm::vec3 worldUp,
                      GLfloat startYaw,
                      GLfloat startPitch,
                      GLfloat startMoveSpeed,
                      GLfloat startTurnSpeed)
{
    cam->position = startPos;
    cam->worldUp = worldUp;
    cam->yaw = startYaw;
    cam->pitch = startPitch;
    cam->front = glm::vec3(0.0f, 0.0f, -1.0f);

    cam->movementSpeed = startMoveSpeed;
    cam->turnSpeed = startTurnSpeed;
    UpdateCamera(cam);
}

glm::mat4 CalculateViewMatrix(Camera* cam)
{
    return glm::lookAt(cam->position, cam->position + cam->front, cam->up);
}

void ControlCameraKeys(Camera * cam, bool* keys, GLfloat deltaTime)
{

    GLfloat velocity = cam->movementSpeed * deltaTime;
    if (keys[GLFW_KEY_LEFT_SHIFT])
    {
        velocity *= 5;
    }
    if (keys[GLFW_KEY_W])
    {
        cam->position += cam->front * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        cam->position -= cam->front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        cam->position -= cam->right * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        cam->position += cam->right * velocity;
    }
    if (keys[GLFW_KEY_SPACE])
    {
        cam->position += glm::vec3(0, 1, 0) * velocity;
    }
}

void ControlCameraMouse(Camera * cam, GLfloat changeX, GLfloat changeY)
{
    changeX *= cam->turnSpeed;
    changeY *= cam->turnSpeed;

    cam->yaw += changeX;
    cam->pitch += changeY;

    if (cam->pitch > 89.0f)
    {
         cam->pitch = 89.0f;
    }
    else if (cam->pitch < -89.0f)
    {
         cam->pitch = -89.0f;
    }
    UpdateCamera(cam);
}
#endif // CAMERAH
