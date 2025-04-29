#pragma once

#include "buffers.h"
#include "types.h"
#include <glad/glad.h>

class VertexArray {
private:
    uint32_t _ID;

    bool _is_bound();

public:
    VertexArray();
    ~VertexArray() { glDeleteVertexArrays(1, &_ID); }

    void link_attrib(int slot, uint32_t size, uint32_t type,
                     unsigned char normalized, uint32_t stride, void* offset);

    void link_generic_attrib(int slot, uint32_t instance_stride, uint32_t size,
                             uint32_t type, unsigned char normalized,
                             uint32_t stride, void* offset);

    void bind();
    void unbind();
};
