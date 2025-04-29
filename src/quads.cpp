#include "quads.h"

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
    _vao->link_attrib(pos_slot, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    _vertex_vbo->write_data(s_instance_vertices, sizeof(s_instance_vertices));
    _vertex_ebo->write_data(s_instance_indices, 6);

    _instance_vbo->bind();
    _vao->link_generic_attrib(color_slot, 1, 4, GL_FLOAT, GL_FALSE,
                              sizeof(ColorF) + sizeof(glm::mat4), (void*)0);

    _vao->link_generic_attrib(model_mat_slot, 1, 4, GL_FLOAT, GL_FALSE,
                              sizeof(ColorF) + sizeof(glm::mat4), (void*)sizeof(ColorF));
    _vao->link_generic_attrib(model_mat_slot + 1, 1, 4, GL_FLOAT, GL_FALSE,
                              sizeof(ColorF) + sizeof(glm::mat4),
                              (void*)(sizeof(ColorF) + (1 * sizeof(glm::vec4))));
    _vao->link_generic_attrib(model_mat_slot + 2, 1, 4, GL_FLOAT, GL_FALSE,
                              sizeof(ColorF) + sizeof(glm::mat4),
                              (void*)(sizeof(ColorF) + (2 * sizeof(glm::vec4))));
    _vao->link_generic_attrib(model_mat_slot + 3, 1, 4, GL_FLOAT, GL_FALSE,
                              sizeof(ColorF) + sizeof(glm::mat4),
                              (void*)(sizeof(ColorF) + (3 * sizeof(glm::vec4))));
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
        data.emplace_back(quad->get_color(), quad->get_transform());
    }

    _instance_vbo->write_data(&data[0], sizeof(QuadData) * data.size());
}

void QuadManager::draw() {
    // TODO: Check if matrices have been modified and then reparse(also add a use check for shader)
    _vao->bind();
    shader->use();
    _reparse_data();

    shader->set_mat_f4("u_projection_mat", _projection_mat);
    shader->set_mat_f4("u_view_mat", _view_mat);
    // TODO: Make wireframe mode toggleable maybe a debug mode?
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, _objects.size());
}
