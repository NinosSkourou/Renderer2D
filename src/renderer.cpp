#include "renderer.h"

Renderer::Renderer(GLFWwindow* window, int width, int height)
    : _window_w(width), _window_h(height) {
        resize_viewport(_window_w, _window_h);
}

Renderer::~Renderer() {}

void Renderer::resize_viewport(int width, int height) {
    _window_w = width;
    _window_h = height;
    glViewport(0, 0, width, height);
}

void Renderer::begin_drawing() {
    glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::end_drawing(GLFWwindow* window) { glfwSwapBuffers(window); }

void Renderer::set_clear_color(Color color) {
    _clear_color = ColorF{color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
}

void Renderer::set_clear_color_float(ColorF color) { _clear_color = color; }
