#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    bool loadFromFile(const std::string& path);
    bool loadFromMemory(const unsigned char* data, int width, int height, int channels);

    void bind(unsigned int unit = 0) const;
    void unbind() const;

    GLuint getId() const { return m_texture; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    void cleanup();

    GLuint m_texture = 0;
    int m_width = 0;
    int m_height = 0;
};
