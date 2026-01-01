#pragma once

#include "Transform.hpp"
#include "graphics/Mesh.hpp"
#include <vector>
#include <memory>
#include <string>

class Model {
public:
    Model() = default;

    void addMesh(std::unique_ptr<Mesh> mesh);

    const std::vector<std::unique_ptr<Mesh>>& getMeshes() const { return m_meshes; }
    Transform& getTransform() { return m_transform; }
    const Transform& getTransform() const { return m_transform; }

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    Transform m_transform;
    std::string m_name;
};
