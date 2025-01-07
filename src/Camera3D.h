#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = 0;
const float PITCH = 0.0f;
const float SPEED = 50;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera3D
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float fov;

public:
    Camera3D(glm::vec3 position = glm::vec3(10.0f, -50.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
    {
        Front = front;
        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;
        fov = ZOOM;
        Position = position;
        Up = glm::vec3(0.0f, 0.0f, 1.0f);
        Yaw = yaw;
        Pitch = pitch;
    }

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

};