#pragma once
#include "buffers.h"
#include "shader.h"
#include "types.h"
#include "vertex_array.h"
#include <glm/matrix.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

class Renderer {
private:
    ColorF _clear_color{0.f, 0.f, 0.f, 0.f};

    uint32_t _window_w = 0;
    uint32_t _window_h = 0;

public:
    Renderer(GLFWwindow* window, int width, int height);
    ~Renderer();

    void resize_viewport(int width, int height);
    // Prepare to draw to screen
    void begin_drawing();
    // Swap buffers and draw current frame
    void end_drawing(GLFWwindow* window);
    // Set color that screen gets cleared with
    void set_clear_color(Color color);
    void set_clear_color_float(ColorF color);

};
