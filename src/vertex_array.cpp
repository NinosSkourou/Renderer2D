#include "vertex_array.h"

uint32_t VertexArray::s_get_type_size(int type) {
    switch(type) {
        case GL_FLOAT:
            return sizeof(float);
        case GL_INT:
            return sizeof(int32_t);
        case GL_UNSIGNED_INT:
            return sizeof(uint32_t);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
    }
    // TODO: Assert error to error handler
    return 0;
}

VertexArray::VertexArray() { glGenVertexArrays(1, &_ID); }

void VertexArray::link_attrib(VertexBuffer& buffer, int type, int slot, uint8_t count, uint32_t stride,
                 uint32_t instance_stride, unsigned char normalized){
    bind();
    buffer.bind();
    glEnableVertexAttribArray(slot);
    glVertexAttribDivisor(slot, instance_stride);
    glVertexAttribPointer(slot, count, type, normalized, stride, (void*)buffer.get_offset());
    buffer.add_offset(s_get_type_size(type)*count);

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
