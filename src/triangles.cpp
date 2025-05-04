#include "triangles.h"

glm::mat4 Triangle::calc_transform_mat() const {
    glm::mat4 transform(1.0f);

    transform = glm::translate(transform, glm::vec3(_position.x, _position.y, 0.0f));
    transform = glm::rotate(transform, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(_scale.x, _scale.y, 1.0f));

    return transform;
}

void TriangleManager::_init() {
    _shader.reset(new Shader{"res/vertex_batch_polygon.glsl", "res/fragment_batch_polygon.glsl"});
    _vao.reset(new VertexArray);
    _vao->bind();
    _vertex_vbo.reset(new VertexBuffer{3000 * sizeof(VertexF), GL_DYNAMIC_DRAW});
    _data_vbo.reset(new VertexBuffer{1000 * (sizeof(glm::mat4) + sizeof(ColorF)), GL_DYNAMIC_DRAW});
}

TriangleManager::TriangleManager() {
    _init();
    _vao->bind();
    _shader->use();
    _vertex_vbo->bind();

    int pos_slot = _shader->get_attrib_slot("in_position");
    int model_mat_slot = _shader->get_attrib_slot("in_model_mat");
    int color_slot = _shader->get_attrib_slot("in_color");

    _vao->link_attrib(*_vertex_vbo, GL_FLOAT, pos_slot, 2,sizeof(VertexF));

    uint32_t stride = sizeof(ColorF) + sizeof(glm::mat4);
    _vao->link_attrib(*_data_vbo, GL_FLOAT, color_slot, 4, stride, 1);

    _vao->link_attrib(*_data_vbo, GL_FLOAT, model_mat_slot, 4, stride, 1);
    _vao->link_attrib(*_data_vbo, GL_FLOAT, model_mat_slot + 1, 4, stride, 1);
    _vao->link_attrib(*_data_vbo, GL_FLOAT, model_mat_slot + 2, 4, stride, 1);
    _vao->link_attrib(*_data_vbo, GL_FLOAT, model_mat_slot + 3, 4, stride, 1);
}

void TriangleManager::add(std::shared_ptr<Triangle> triangle) {
    triangle->set_id(_next_id);
    _objects[_next_id] = triangle;

    _next_id++;
}

void TriangleManager::remove(uint32_t id) {
    auto iter = _objects.find(id);
    if (iter != _objects.end()) {
        _objects.erase(iter);
    }
}

void TriangleManager::_reparse_data() {
    std::vector<TriangleData> data;
    std::vector<VertexF> vertices;
    data.reserve(_objects.size());
    vertices.reserve(3 * _objects.size());

    for (auto& [id, object] : _objects) {
        data.emplace_back(object->calc_transform_mat(), object->get_color());
        
        const auto& verts = object->get_vertices();
        for(int i = 0; i < 3; i++)
            vertices.emplace_back(verts[i]);
    }

    _vao->bind();
    _vertex_vbo->bind();
    _vertex_vbo->write_data(&vertices[0], sizeof(VertexF) * vertices.size());

    _data_vbo->bind();
    _data_vbo->write_data(&data[0], sizeof(TriangleData) * data.size());

}

void TriangleManager::draw() {
    _vao->bind();
    _shader->use();

    _reparse_data();

    _shader->set_mat_f4("u_projection_mat", _projection_mat);
    _shader->set_mat_f4("u_view_mat", _view_mat);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, _objects.size() * 3);
}
