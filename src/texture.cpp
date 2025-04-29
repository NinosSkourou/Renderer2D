#include "texture.h"

Texture::Texture(const unsigned char* data, int width, int height)
    : _width(width), _height(height) {
    glGenTextures(1, &_ID);
    bind();

    // TODO: Define a struct that contains Texture configuration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    unbind();
}

Texture::~Texture() { glDeleteTextures(1, &_ID); }
