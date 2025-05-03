#include "quads.h"

glm::mat4 Quad::calc_transform_mat() const {
    glm::mat4 transform(1.0f);

    transform = glm::translate(transform, glm::vec3(_position.x, _position.y, 0.0f));
    transform = glm::rotate(transform, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(_scale.x, _scale.y, 1.0f));

    return transform;
}

void QuadManager::_init() {
    shader = new Shader{"res/vertex_batch_polygon.glsl", "res/fragment_batch_polygon.glsl"};
    _vao = new VertexArray;
    _vao->bind();
    _vertex_vbo = new VertexBuffer{sizeof(s_instance_vertices), GL_STATIC_DRAW};
    _instance_vbo = new VertexBuffer{1000 * sizeof(QuadData), GL_DYNAMIC_DRAW};
    _vertex_ebo = new ElementBuffer{sizeof(s_instance_indices), GL_STATIC_DRAW};
    _vertex_ebo->bind();
}

QuadManager::~QuadManager() {
    delete shader;
    delete _vao;
    delete _vertex_vbo;
    delete _vertex_ebo;
    delete _instance_vbo;
}

QuadManager::QuadManager() {
    _init();
    _vao->bind();
    shader->use();
    int pos_slot = shader->get_attrib_slot("in_position");
    int model_mat_slot = shader->get_attrib_slot("in_model_mat");
    int color_slot = shader->get_attrib_slot("in_color");
    // TODO: maybe implement offset and stride into VertexArray class

    _vertex_vbo->bind();
    _vao->link_attrib(*_vertex_vbo, GL_FLOAT, pos_slot, 2, 2 * sizeof(float));

    _vertex_vbo->write_data(s_instance_vertices, sizeof(s_instance_vertices));
    _vertex_ebo->write_data(s_instance_indices, 6);

    uint32_t stride = sizeof(ColorF) + sizeof(glm::mat4);
    _vao->link_attrib(*_instance_vbo, GL_FLOAT, color_slot, 4, stride, 1);

    _vao->link_attrib(*_instance_vbo, GL_FLOAT, model_mat_slot, 4, stride, 1);
    _vao->link_attrib(*_instance_vbo, GL_FLOAT, model_mat_slot + 1, 4, stride, 1);
    _vao->link_attrib(*_instance_vbo, GL_FLOAT, model_mat_slot + 2, 4, stride, 1);
    _vao->link_attrib(*_instance_vbo, GL_FLOAT, model_mat_slot + 3, 4, stride, 1);
}

void QuadManager::add(std::shared_ptr<Quad> quad) {
    quad->set_id(_next_id);
    _objects[_next_id] = quad;

    _next_id++;
}

void QuadManager::remove(uint32_t id) {
    auto iter = _objects.find(id);
    if (iter != _objects.end()) {
        _objects.erase(iter);
    }
}

void QuadManager::_reparse_data() {
    std::vector<QuadData> data;
    data.reserve(_objects.size());

    for (auto& pair : _objects) {
        auto& quad = pair.second;
        quad->calc_transform_mat();
        data.emplace_back(quad->get_color(), quad->calc_transform_mat());
    }

    _instance_vbo->write_data(&data[0], sizeof(QuadData) * data.size());
}

void QuadManager::draw() {
    _vao->bind();
    shader->use();
    _reparse_data();

    shader->set_mat_f4("u_projection_mat", _projection_mat);
    shader->set_mat_f4("u_view_mat", _view_mat);
    // TODO: Make wireframe mode toggleable maybe a debug mode?
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, _objects.size());
}
