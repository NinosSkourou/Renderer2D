#pragma once

#include <glad/glad.h>

class VertexBuffer {
private:
    uint32_t _ID;

    uint64_t _accum_offset = 0;

public:
    VertexBuffer(uint32_t size, uint32_t draw_mode);
    ~VertexBuffer();

    void write_data(const void* data, uint32_t size);
    void write_subdata(uint32_t offset, const void* data, uint32_t size);

    void add_offset(uint64_t size) { _accum_offset += size; }
    uint64_t get_offset() const { return _accum_offset; }

    void bind() const;
    void unbind() const;
};

class ElementBuffer {
private:
    uint32_t _ID;

public:
    ElementBuffer(uint32_t amount, uint32_t draw_mode);
    ~ElementBuffer();

    void write_data(const uint32_t* indices, uint32_t count);
    void write_subdata(uint32_t offset, const uint32_t* data, uint32_t count);

    void bind() const;
    void unbind() const;
};
