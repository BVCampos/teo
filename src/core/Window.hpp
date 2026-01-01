#pragma once

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool init();
    void swapBuffers();
    void pollEvents();

    bool shouldClose() const { return m_shouldClose; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getAspectRatio() const { return static_cast<float>(m_width) / m_height; }
    SDL_Window* getHandle() const { return m_window; }

    float getDeltaTime() const { return m_deltaTime; }

    bool isKeyDown(SDL_Scancode key) const;
    void getMouseDelta(int& dx, int& dy) const;
    void setMouseCapture(bool capture);

private:
    void handleEvent(const SDL_Event& event);

    std::string m_title;
    int m_width;
    int m_height;
    bool m_shouldClose = false;

    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;

    Uint64 m_lastTime = 0;
    float m_deltaTime = 0.0f;

    int m_mouseDeltaX = 0;
    int m_mouseDeltaY = 0;
    bool m_mouseCaptured = false;
};
