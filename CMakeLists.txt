cmake_minimum_required(VERSION 3.20)
project(teo VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# GLM (header-only math library)
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLAD (OpenGL loader) - we'll include sources directly
add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c
)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/third_party/glad/include)

# tinygltf is header-only, just need to set include path

# Main executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core/Window.cpp
    src/graphics/Shader.cpp
    src/graphics/Mesh.cpp
    src/graphics/Texture.cpp
    src/scene/Transform.cpp
    src/scene/Camera.cpp
    src/scene/Model.cpp
    src/loader/GLTFLoader.cpp
    src/graphics/Renderer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/tinygltf
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    OpenGL::GL
    glad
    glm::glm
)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
