#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Texture;

struct Material {
    glm::vec4 baseColorFactor = glm::vec4(1.0f);
    std::shared_ptr<Texture> baseColorTexture;
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void setup(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void draw() const;

    void setMaterial(const Material& material) { m_material = material; }
    const Material& getMaterial() const { return m_material; }

private:
    void cleanup();

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLsizei m_indexCount = 0;
    Material m_material;
};
