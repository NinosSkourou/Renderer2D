#include "vertex_array.h"

VertexArray::VertexArray() { glGenVertexArrays(1, &_ID); }

void VertexArray::link_attrib(int slot, uint32_t size, uint32_t type, unsigned char normalized,
                              uint32_t stride, void* offset) {
    bind();
    glEnableVertexAttribArray(slot);
    glVertexAttribPointer(slot, size, type, normalized, stride, offset);
}

void VertexArray::link_generic_attrib(int slot, uint32_t instance_stride, uint32_t size,
                                      uint32_t type, unsigned char normalized, uint32_t stride,
                                      void* offset) {
    bind();
    glEnableVertexAttribArray(slot);
    glVertexAttribPointer(slot, size, type, normalized, stride, offset);
    glVertexAttribDivisor(slot, instance_stride);
}

bool VertexArray::_is_bound() {
    int bound_buf;
    glGetIntegerv(GL_VERTEX_ARRAY, &bound_buf);
    return (uint32_t)bound_buf == _ID;
}

void VertexArray::bind() {
    if (!_is_bound()) {
        glBindVertexArray(_ID);
    }
}

void VertexArray::unbind() {
    if (_is_bound()) {
        glBindVertexArray(0);
    }
}
