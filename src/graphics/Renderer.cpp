#include "Renderer.hpp"
#include "Texture.hpp"
#include <glad/glad.h>

Renderer::Renderer() {}

bool Renderer::init() {
    if (!m_shader.loadFromFiles("shaders/basic.vert", "shaders/basic.frag")) {
        return false;
    }
    return true;
}

void Renderer::render(const Camera& camera, const std::vector<std::unique_ptr<Model>>& models) {
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.use();

    m_shader.setMat4("view", camera.getViewMatrix());
    m_shader.setMat4("projection", camera.getProjectionMatrix());
    m_shader.setVec3("viewPos", camera.getPosition());

    m_shader.setVec3("lightDir", m_lightDir);
    m_shader.setVec3("lightColor", m_lightColor);
    m_shader.setVec3("ambientColor", m_ambientColor);

    for (const auto& model : models) {
        const auto& transform = model->getTransform();
        m_shader.setMat4("model", transform.getMatrix());
        m_shader.setMat3("normalMatrix", transform.getNormalMatrix());

        for (const auto& mesh : model->getMeshes()) {
            const auto& material = mesh->getMaterial();

            m_shader.setVec4("baseColorFactor", material.baseColorFactor);

            if (material.baseColorTexture) {
                material.baseColorTexture->bind(0);
                m_shader.setInt("baseColorTexture", 0);
                m_shader.setInt("hasTexture", 1);
            } else {
                m_shader.setInt("hasTexture", 0);
            }

            mesh->draw();
        }
    }
}

void Renderer::setClearColor(const glm::vec4& color) {
    m_clearColor = color;
}

void Renderer::setLightDirection(const glm::vec3& dir) {
    m_lightDir = glm::normalize(dir);
}

void Renderer::setLightColor(const glm::vec3& color) {
    m_lightColor = color;
}

void Renderer::setAmbientColor(const glm::vec3& color) {
    m_ambientColor = color;
}
