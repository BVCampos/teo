#pragma once

#include "scene/Model.hpp"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

class Texture;

class GLTFLoader {
public:
    GLTFLoader() = default;

    std::unique_ptr<Model> load(const std::string& path);

private:
    std::string m_basePath;
    std::unordered_map<int, std::shared_ptr<Texture>> m_textureCache;
};
