#include "core/Window.hpp"
#include "graphics/Renderer.hpp"
#include "scene/Camera.hpp"
#include "loader/GLTFLoader.hpp"

#include <iostream>
#include <vector>
#include <memory>

int main(int argc, char* argv[]) {
    Window window("Teo - OpenGL glTF Renderer", 1280, 720);

    if (!window.init()) {
        return -1;
    }

    Renderer renderer;
    if (!renderer.init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    Camera camera(45.0f, window.getAspectRatio());
    camera.setPosition(glm::vec3(0.0f, 1.0f, 5.0f));

    std::vector<std::unique_ptr<Model>> models;

    // Load models from command line arguments
    GLTFLoader loader;
    for (int i = 1; i < argc; ++i) {
        auto model = loader.load(argv[i]);
        if (model) {
            models.push_back(std::move(model));
        }
    }

    if (models.empty()) {
        std::cout << "Usage: " << argv[0] << " <model.gltf/glb> [model2.gltf/glb] ..." << std::endl;
        std::cout << "No models loaded. Displaying empty scene." << std::endl;
    }

    const float cameraSpeed = 5.0f;
    const float mouseSensitivity = 0.1f;

    std::cout << "\nControls:" << std::endl;
    std::cout << "  WASD - Move camera" << std::endl;
    std::cout << "  Mouse - Look around (click to capture)" << std::endl;
    std::cout << "  Space/Shift - Move up/down" << std::endl;
    std::cout << "  ESC - Release mouse / Exit" << std::endl;

    while (!window.shouldClose()) {
        window.pollEvents();

        float dt = window.getDeltaTime();

        // Camera movement
        glm::vec3 moveDir(0.0f);
        if (window.isKeyDown(SDL_SCANCODE_W)) moveDir.z += 1.0f;
        if (window.isKeyDown(SDL_SCANCODE_S)) moveDir.z -= 1.0f;
        if (window.isKeyDown(SDL_SCANCODE_A)) moveDir.x -= 1.0f;
        if (window.isKeyDown(SDL_SCANCODE_D)) moveDir.x += 1.0f;
        if (window.isKeyDown(SDL_SCANCODE_SPACE)) moveDir.y += 1.0f;
        if (window.isKeyDown(SDL_SCANCODE_LSHIFT)) moveDir.y -= 1.0f;

        camera.processKeyboard(moveDir, cameraSpeed * dt);

        // Mouse look
        int dx, dy;
        window.getMouseDelta(dx, dy);
        camera.processMouseMovement(static_cast<float>(dx), static_cast<float>(dy), mouseSensitivity);

        // Update camera aspect ratio on window resize
        camera.setAspect(window.getAspectRatio());

        // Render
        renderer.render(camera, models);

        window.swapBuffers();
    }

    return 0;
}
