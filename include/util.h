#pragma once

#include <glad/glad.h>
#include <iostream>
#include <signal.h>

#define ASSERT(x)                                                                                  \
    if (!(x))                                                                                      \
        raise(SIGTRAP);

#define GL_CALL(x)                                                                                 \
    gl_clear_error();                                                                              \
    x;                                                                                             \
    ASSERT(gl_log_call())

static void gl_clear_error() {
    while (glGetError() != GL_NO_ERROR) {
    };
}

static bool gl_log_call() {
    while (GLenum error = glGetError()) {
        std::cout << "OPENGL ERROR: " << error << std::endl;
        return false;
    }
    return true;
}
