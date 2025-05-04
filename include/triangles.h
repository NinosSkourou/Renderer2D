#pragma once

#include "buffers.h"
#include "shader.h"
#include "types.h"
#include "vertex_array.h"
#include <cstdint>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

struct TriangleData {
    ColorF color{};
    glm::mat4 transform_mat{1.0f};

    TriangleData(const glm::mat4& t, const ColorF& c) : color(c), transform_mat(t) {}
};

class Triangle {
private:
    uint32_t _id;

    VertexF _vertices[3];
    ColorF _color;

    glm::vec2 _scale{1.0f};
    glm::vec2 _position{0.0f};
    float _rotation = 0.0f;

public:
    Triangle(const VertexF* vertices, ColorF color) : _color(color) {
        memcpy(_vertices, vertices, sizeof(VertexF) * 3);
    };
    ~Triangle() {};

    void set_id(uint32_t id) { _id = id; }
    uint32_t get_id() const { return _id; }

    void set_position(glm::vec2 position) { _position = position; }
    void set_rotation(float rotation) { _rotation = rotation; }
    void set_scale(glm::vec2 scale) { _scale = scale; }

    glm::mat4 calc_transform_mat() const;
    const ColorF& get_color() const { return _color; }
    const VertexF* get_vertices() const { return _vertices; }
};

class TriangleManager {
private:
    glm::mat4 _projection_mat, _view_mat;

    uint32_t _next_id = 1;

    std::unordered_map<uint32_t, std::shared_ptr<Triangle>> _objects;

    std::unique_ptr<Shader> _shader = nullptr;
    std::unique_ptr<VertexArray> _vao = nullptr;
    std::unique_ptr<VertexBuffer> _vertex_vbo = nullptr;
    std::unique_ptr<VertexBuffer> _data_vbo = nullptr;

    void _init();

    void _reparse_data();

public:
    TriangleManager();
    ~TriangleManager() {};

    void add(std::shared_ptr<Triangle> triangle);

    void remove(uint32_t id);

    void draw();

    void set_projection_mat(const glm::mat4& mat) { _projection_mat = mat; }
    void set_view_mat(const glm::mat4& mat) { _view_mat = mat; }
};
