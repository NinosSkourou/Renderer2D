#include "buffers.h"

static bool s_is_buffer_bound(uint32_t type, int ID) {
    int bound_buf;
    glGetIntegerv(type, &bound_buf);

    if (bound_buf != ID) {
        return false;
    }
    return true;
}

VertexBuffer::VertexBuffer(uint32_t size, uint32_t draw_mode) {
    glGenBuffers(1, &_ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, draw_mode);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &_ID); }

void VertexBuffer::write_data(const void* data, uint32_t size) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::write_subdata(uint32_t offset, const void* data, uint32_t size) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::bind() const {
    if (!s_is_buffer_bound(GL_ARRAY_BUFFER_BINDING, _ID)) {
        glBindBuffer(GL_ARRAY_BUFFER, _ID);
    }
}

void VertexBuffer::unbind() const {
    if (s_is_buffer_bound(GL_ARRAY_BUFFER_BINDING, _ID)) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

ElementBuffer::ElementBuffer(uint32_t size, uint32_t draw_mode) {
    glGenBuffers(1, &_ID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, draw_mode);
}

void ElementBuffer::bind() const {
    if (!s_is_buffer_bound(GL_ELEMENT_ARRAY_BUFFER_BINDING, _ID)) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID);
    }
}

void ElementBuffer::unbind() const {
    if (s_is_buffer_bound(GL_ELEMENT_ARRAY_BUFFER_BINDING, _ID)) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void ElementBuffer::write_data(const uint32_t* indices, uint32_t count) {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * count, indices);
}

void ElementBuffer::write_subdata(uint32_t offset, const uint32_t* indices, uint32_t count) {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(uint32_t) * count, indices);
}

ElementBuffer::~ElementBuffer() { glDeleteBuffers(1, &_ID); }
