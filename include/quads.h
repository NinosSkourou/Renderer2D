#pragma once

#include "buffers.h"
#include "shader.h"
#include "types.h"
#include "vertex_array.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

struct QuadData {
    QuadData(ColorF c, glm::mat4 t) : color(c), transform(t) {}

    ColorF color;
    glm::mat4 transform;
};

class Quad {
private:
    uint32_t _id;

    ColorF _color;
    glm::mat4 _model_transform{1.0f};

public:
    Quad(ColorF color) : _color(color) {}
    ~Quad() {};

    void set_id(uint32_t id) { _id = id; }
    uint32_t get_id() const { return _id; }

    void set_position(glm::vec2 position) {
        _model_transform[3][0] = position.x;
        _model_transform[3][1] = position.y;
    }

    const glm::mat4& get_transform() const { return _model_transform; }
    const ColorF& get_color() const { return _color; }
};

class QuadManager {
private:
    glm::mat4 _projection_mat, _view_mat;

    static constexpr float s_instance_vertices[] = {
        0.0f, 0.0f, 100.0f, 0.0f, 100.0f, 100.0f, 0.0f, 100.0f};

    static constexpr uint32_t s_instance_indices[] = {0, 1, 2, 0, 2, 3};

    uint32_t _next_id = 1;

    std::unordered_map<uint32_t, std::shared_ptr<Quad>> _objects;

    Shader* shader = nullptr;
    VertexArray* _vao = nullptr;
    VertexBuffer* _vertex_vbo = nullptr;
    VertexBuffer* _instance_vbo = nullptr;
    ElementBuffer* _vertex_ebo = nullptr;

    void _init();

    void _reparse_data();

public:
    QuadManager();
    ~QuadManager();

    void add(std::shared_ptr<Quad> quad);

    void remove(uint32_t id);

    void draw();

    void set_projection_mat(const glm::mat4& mat) { _projection_mat = mat; }
    void set_view_mat(const glm::mat4& mat) { _view_mat = mat; }
};
