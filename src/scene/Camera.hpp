#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov = 45.0f, float aspect = 16.0f/9.0f, float near = 0.1f, float far = 1000.0f);

    void setPosition(const glm::vec3& pos);
    void setFov(float fov);
    void setAspect(float aspect);
    void setClipPlanes(float near, float far);

    const glm::vec3& getPosition() const { return m_position; }
    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }

    void processMouseMovement(float xOffset, float yOffset, float sensitivity = 0.1f);
    void processKeyboard(const glm::vec3& direction, float speed);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    glm::vec3 getForward() const { return m_front; }
    glm::vec3 getRight() const { return m_right; }
    glm::vec3 getUp() const { return m_up; }

private:
    void updateVectors();
    void updateProjection();

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    glm::mat4 m_projection;
};
