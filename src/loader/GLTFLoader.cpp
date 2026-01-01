#include "GLTFLoader.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include <iostream>
#include <filesystem>

std::unique_ptr<Model> GLTFLoader::load(const std::string& path) {
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    m_basePath = std::filesystem::path(path).parent_path().string();
    if (!m_basePath.empty()) {
        m_basePath += "/";
    }

    bool success = false;
    if (path.ends_with(".glb")) {
        success = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, path);
    } else {
        success = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, path);
    }

    if (!warn.empty()) {
        std::cerr << "glTF warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "glTF error: " << err << std::endl;
    }

    if (!success) {
        std::cerr << "Failed to load glTF: " << path << std::endl;
        return nullptr;
    }

    auto model = std::make_unique<Model>();
    model->setName(std::filesystem::path(path).stem().string());

    m_textureCache.clear();

    for (const auto& gltfMesh : gltfModel.meshes) {
        for (const auto& primitive : gltfMesh.primitives) {
            if (primitive.mode != TINYGLTF_MODE_TRIANGLES) {
                continue;
            }

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            // Get accessors
            const float* positions = nullptr;
            const float* normals = nullptr;
            const float* texCoords = nullptr;
            size_t vertexCount = 0;

            // Position
            if (primitive.attributes.count("POSITION")) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];

                positions = reinterpret_cast<const float*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset
                );
                vertexCount = accessor.count;
            }

            // Normal
            if (primitive.attributes.count("NORMAL")) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];

                normals = reinterpret_cast<const float*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset
                );
            }

            // Texture coordinates
            if (primitive.attributes.count("TEXCOORD_0")) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];

                texCoords = reinterpret_cast<const float*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset
                );
            }

            // Build vertices
            vertices.reserve(vertexCount);
            for (size_t i = 0; i < vertexCount; ++i) {
                Vertex v;
                v.position = glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);

                if (normals) {
                    v.normal = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
                } else {
                    v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }

                if (texCoords) {
                    v.texCoord = glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]);
                } else {
                    v.texCoord = glm::vec2(0.0f);
                }

                vertices.push_back(v);
            }

            // Indices
            if (primitive.indices >= 0) {
                const auto& accessor = gltfModel.accessors[primitive.indices];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];

                indices.reserve(accessor.count);

                const void* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        const uint16_t* data = static_cast<const uint16_t*>(dataPtr);
                        for (size_t i = 0; i < accessor.count; ++i) {
                            indices.push_back(data[i]);
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                        const uint32_t* data = static_cast<const uint32_t*>(dataPtr);
                        for (size_t i = 0; i < accessor.count; ++i) {
                            indices.push_back(data[i]);
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        const uint8_t* data = static_cast<const uint8_t*>(dataPtr);
                        for (size_t i = 0; i < accessor.count; ++i) {
                            indices.push_back(data[i]);
                        }
                        break;
                    }
                }
            } else {
                // No indices, generate sequential
                indices.reserve(vertexCount);
                for (size_t i = 0; i < vertexCount; ++i) {
                    indices.push_back(static_cast<unsigned int>(i));
                }
            }

            // Create mesh
            auto mesh = std::make_unique<Mesh>();
            mesh->setup(vertices, indices);

            // Material
            Material material;
            if (primitive.material >= 0) {
                const auto& gltfMat = gltfModel.materials[primitive.material];
                const auto& pbr = gltfMat.pbrMetallicRoughness;

                material.baseColorFactor = glm::vec4(
                    pbr.baseColorFactor[0],
                    pbr.baseColorFactor[1],
                    pbr.baseColorFactor[2],
                    pbr.baseColorFactor[3]
                );

                // Load base color texture
                if (pbr.baseColorTexture.index >= 0) {
                    int texIndex = pbr.baseColorTexture.index;

                    if (m_textureCache.count(texIndex)) {
                        material.baseColorTexture = m_textureCache[texIndex];
                    } else {
                        const auto& gltfTex = gltfModel.textures[texIndex];
                        if (gltfTex.source >= 0) {
                            const auto& image = gltfModel.images[gltfTex.source];

                            auto texture = std::make_shared<Texture>();

                            if (!image.image.empty()) {
                                // Embedded image data
                                texture->loadFromMemory(
                                    image.image.data(),
                                    image.width,
                                    image.height,
                                    image.component
                                );
                            } else if (!image.uri.empty()) {
                                // External file
                                texture->loadFromFile(m_basePath + image.uri);
                            }

                            m_textureCache[texIndex] = texture;
                            material.baseColorTexture = texture;
                        }
                    }
                }
            }

            mesh->setMaterial(material);
            model->addMesh(std::move(mesh));
        }
    }

    std::cout << "Loaded glTF: " << path << " (" << model->getMeshes().size() << " meshes)" << std::endl;

    return model;
}
