#pragma once

#include "buffers.h"
#include "types.h"
#include <glad/glad.h>

class VertexArray {
private:
    uint32_t _ID;

    bool _is_bound();

    static uint32_t s_get_type_size(int type);

public:
    VertexArray();
    ~VertexArray() { glDeleteVertexArrays(1, &_ID); }

    void link_attrib(VertexBuffer& buffer,int type, int slot, uint8_t count, uint32_t stride,
                     uint32_t instance_stride = 0, unsigned char normalized = GL_FALSE);

    void bind();
    void unbind();
};
