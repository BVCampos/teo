#include "Window.hpp"
#include <glad/glad.h>
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

Window::~Window() {
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

bool Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,
        m_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!gladLoadGLLoader((void*(*)(const char*))SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    std::cout << "OpenGL Info:" << std::endl;
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_lastTime = SDL_GetPerformanceCounter();

    return true;
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(m_window);

    Uint64 currentTime = SDL_GetPerformanceCounter();
    m_deltaTime = static_cast<float>(currentTime - m_lastTime) / SDL_GetPerformanceFrequency();
    m_lastTime = currentTime;
}

void Window::pollEvents() {
    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handleEvent(event);
    }
}

void Window::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            m_shouldClose = true;
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                m_width = event.window.data1;
                m_height = event.window.data2;
                glViewport(0, 0, m_width, m_height);
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (m_mouseCaptured) {
                    setMouseCapture(false);
                } else {
                    m_shouldClose = true;
                }
            }
            break;

        case SDL_MOUSEMOTION:
            if (m_mouseCaptured) {
                m_mouseDeltaX = event.motion.xrel;
                m_mouseDeltaY = event.motion.yrel;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!m_mouseCaptured && event.button.button == SDL_BUTTON_LEFT) {
                setMouseCapture(true);
            }
            break;
    }
}

bool Window::isKeyDown(SDL_Scancode key) const {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[key];
}

void Window::getMouseDelta(int& dx, int& dy) const {
    dx = m_mouseDeltaX;
    dy = m_mouseDeltaY;
}

void Window::setMouseCapture(bool capture) {
    m_mouseCaptured = capture;
    SDL_SetRelativeMouseMode(capture ? SDL_TRUE : SDL_FALSE);
}
