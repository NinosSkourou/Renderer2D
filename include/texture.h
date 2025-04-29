#pragma once

#include <glad/glad.h>

// ALL TODO

class Texture {
private:
    uint32_t _ID;
    int _width, _height;

public:
    Texture(const unsigned char* data, int width, int height);
    ~Texture();

    void bind(uint8_t slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, _ID);
    }

    void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
};
