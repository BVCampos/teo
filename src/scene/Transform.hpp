#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    Transform() = default;

    void setPosition(const glm::vec3& pos) { m_position = pos; m_dirty = true; }
    void setRotation(const glm::quat& rot) { m_rotation = rot; m_dirty = true; }
    void setScale(const glm::vec3& scale) { m_scale = scale; m_dirty = true; }

    const glm::vec3& getPosition() const { return m_position; }
    const glm::quat& getRotation() const { return m_rotation; }
    const glm::vec3& getScale() const { return m_scale; }

    void translate(const glm::vec3& delta) { m_position += delta; m_dirty = true; }
    void rotate(const glm::quat& delta) { m_rotation = delta * m_rotation; m_dirty = true; }

    const glm::mat4& getMatrix() const {
        if (m_dirty) {
            updateMatrix();
        }
        return m_matrix;
    }

    glm::mat3 getNormalMatrix() const {
        return glm::transpose(glm::inverse(glm::mat3(getMatrix())));
    }

    glm::vec3 getForward() const {
        return glm::normalize(m_rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 getRight() const {
        return glm::normalize(m_rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 getUp() const {
        return glm::normalize(m_rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

private:
    void updateMatrix() const {
        m_matrix = glm::translate(glm::mat4(1.0f), m_position);
        m_matrix *= glm::mat4_cast(m_rotation);
        m_matrix = glm::scale(m_matrix, m_scale);
        m_dirty = false;
    }

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);

    mutable glm::mat4 m_matrix = glm::mat4(1.0f);
    mutable bool m_dirty = true;
};
