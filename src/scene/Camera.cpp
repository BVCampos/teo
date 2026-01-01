#include "Camera.hpp"
#include <algorithm>

Camera::Camera(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far) {
    updateVectors();
    updateProjection();
}

void Camera::setPosition(const glm::vec3& pos) {
    m_position = pos;
}

void Camera::setFov(float fov) {
    m_fov = fov;
    updateProjection();
}

void Camera::setAspect(float aspect) {
    m_aspect = aspect;
    updateProjection();
}

void Camera::setClipPlanes(float near, float far) {
    m_near = near;
    m_far = far;
    updateProjection();
}

void Camera::processMouseMovement(float xOffset, float yOffset, float sensitivity) {
    m_yaw += xOffset * sensitivity;
    m_pitch -= yOffset * sensitivity;

    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    updateVectors();
}

void Camera::processKeyboard(const glm::vec3& direction, float speed) {
    m_position += m_front * direction.z * speed;
    m_position += m_right * direction.x * speed;
    m_position += m_worldUp * direction.y * speed;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return m_projection;
}

void Camera::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::updateProjection() {
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}
