#include "shader.h"

static void s_load_file(const char* path, std::string* contents) {
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream file_stream;

        file_stream << file.rdbuf();

        file.close();

        contents->clear();
        *contents = file_stream.str();
    } catch (std::ifstream::failure const& error) {
        std::cout << "FILE NOT SUCCESSFULLY READ: " << error.what() << std::endl;
    }
}

uint32_t Shader::s_compile_shader(uint32_t type, const std::string& source) {
    uint32_t ID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(ID, 1, &src, nullptr);
    glCompileShader(ID);

    int success;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
        std::cout << "FAILED TO COMPILE SHADER "
                  << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
        if (length > 0) {
            char* info_log = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(ID, length, &length, info_log);
            std::cout << info_log << std::endl;
        } else {
            std::cout << "NO REGARDING FAILURE FOR FAILURE (you're fucked ._.)" << std::endl;
        }
        glDeleteShader(ID);
        return 0;
    }

    return ID;
}

uint32_t Shader::s_link_program(uint32_t vertex, uint32_t fragment) {
    uint32_t ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
        std::cout << "SHADER PROGRAM FAILED TO LINK" << std::endl;
        if (length > 0) {
            char* info_log = (char*)alloca(length * sizeof(char));
            glGetProgramInfoLog(ID, length, &length, info_log);
            std::cout << info_log << std::endl;
        } else {
            std::cout << "NO INFO PROVIDED FOR FAILURE (you're fucked ._.)" << std::endl;
        }
    }

    return ID;
}

std::optional<int> Shader::_get_location(const char* u_name) {
    int location;

    if (_uni_cache.find(u_name) != _uni_cache.end()) {
        location = _uni_cache[u_name];
        return location;
    } else {
        location = glGetUniformLocation(_ID, u_name);
    }

    _uni_cache[u_name] = location;
    if (location == -1) {
        std::cout << "FAILED TO LOCATE UNIFORM: " << u_name << std::endl;
        return std::nullopt;
    }
    return location;
}

int Shader::get_attrib_slot(const char* name) {
    auto iter = _attrib_slot_cache.find(name);
    if (iter != _attrib_slot_cache.end()) {
        return _attrib_slot_cache[name];
    }
    use();
    int slot = glGetAttribLocation(_ID, name);
    if (slot == -1) {
        std::cout << "FAILED TO LOCATE ATTRIB SLOT: " << name << std::endl;
        return -1;
    }

    _attrib_slot_cache[name] = slot;
    return slot;
}

Shader::Shader(const char* vertex_path, const char* frag_path) {
    uint32_t vertex_shader, fragment_shader;
    std::string shader_source;
    s_load_file(vertex_path, &shader_source);
    vertex_shader = s_compile_shader(GL_VERTEX_SHADER, shader_source);
    s_load_file(frag_path, &shader_source);
    fragment_shader = s_compile_shader(GL_FRAGMENT_SHADER, shader_source);

    _ID = s_link_program(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() { glDeleteProgram(_ID); }

void Shader::use() { glUseProgram(_ID); }

void Shader::set_f1(const char* u_name, float v) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform1f(location.value(), v);
    }
}

void Shader::set_f2(const char* u_name, float v1, float v2) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform2f(location.value(), v1, v2);
    }
}

void Shader::set_f3(const char* u_name, float v1, float v2, float v3) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform3f(location.value(), v1, v2, v3);
    }
}

void Shader::set_f4(const char* u_name, float v1, float v2, float v3, float v4) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform4f(location.value(), v1, v2, v3, v4);
    }
}

void Shader::set_array_f1(const char* u_name, const float* v, uint32_t count) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform1fv(location.value(), count, v);
    }
}
void Shader::set_array_f2(const char* u_name, const float* v, uint32_t count) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform2fv(location.value(), count, v);
    }
}
void Shader::set_array_f3(const char* u_name, const float* v, uint32_t count) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform3fv(location.value(), count, v);
    }
}
void Shader::set_array_f4(const char* u_name, const float* v, uint32_t count) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform4fv(location.value(), count, v);
    }
}

void Shader::set_mat_f3(const char* u_name, glm::mat3 mat) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniformMatrix3fv(location.value(), 1, GL_FALSE, (float*)&mat[0]);
    }
}

void Shader::set_mat_f4(const char* u_name, glm::mat4 mat) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniformMatrix4fv(location.value(), 1, GL_FALSE, (float*)&mat[0]);
    }
}

void Shader::set_i1(const char* u_name, int v) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform1i(location.value(), v);
    }
}

void Shader::set_i2(const char* u_name, int v1, int v2) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform2i(location.value(), v1, v2);
    }
}

void Shader::set_i3(const char* u_name, int v1, int v2, int v3) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform3i(location.value(), v1, v2, v3);
    }
}

void Shader::set_i4(const char* u_name, int v1, int v2, int v3, int v4) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform4i(location.value(), v1, v2, v3, v4);
    }
}

void Shader::set_u1(const char* u_name, uint32_t v) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform1ui(location.value(), v);
    }
}

void Shader::set_u2(const char* u_name, uint32_t v1, uint32_t v2) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform2ui(location.value(), v1, v2);
    }
}

void Shader::set_u3(const char* u_name, uint32_t v1, uint32_t v2, uint32_t v3) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform3ui(location.value(), v1, v2, v3);
    }
}

void Shader::set_u4(const char* u_name, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) {
    std::optional<int> location = _get_location(u_name);
    if (location.has_value()) {
        glUniform4ui(location.value(), v1, v2, v3, v4);
    }
}
