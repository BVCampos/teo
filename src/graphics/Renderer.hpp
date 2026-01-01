#pragma once

#include "Shader.hpp"
#include "scene/Camera.hpp"
#include "scene/Model.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Renderer {
public:
    Renderer();

    bool init();
    void render(const Camera& camera, const std::vector<std::unique_ptr<Model>>& models);

    void setClearColor(const glm::vec4& color);
    void setLightDirection(const glm::vec3& dir);
    void setLightColor(const glm::vec3& color);
    void setAmbientColor(const glm::vec3& color);

private:
    Shader m_shader;

    glm::vec4 m_clearColor = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
    glm::vec3 m_lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    glm::vec3 m_lightColor = glm::vec3(1.0f);
    glm::vec3 m_ambientColor = glm::vec3(0.15f);
};
