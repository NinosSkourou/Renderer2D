#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

// TODO: ADD A CHECK IF PROGRAME IS ALREADY IN USE BEFORE USING(maybe rename use to bind)

class Shader {
private:
    uint32_t _ID;

    std::unordered_map<std::string, int> _uni_cache;
    std::unordered_map<std::string, int> _attrib_slot_cache;

    static uint32_t s_compile_shader(uint32_t type, const std::string& source);
    static uint32_t s_link_program(uint32_t vertex, uint32_t fragment);

    std::optional<int> _get_location(const char* u_name);

public:
    Shader(const char* vert_code, const char* frag_code);
    ~Shader();

    void use();

    int get_attrib_slot(const char* name);

    // Kill me pls ._.
    void set_f1(const char* u_name, float v);
    void set_f2(const char* u_name, float v1, float v2);
    void set_f3(const char* u_name, float v1, float v2, float v3);
    void set_f4(const char* u_name, float v1, float v2, float v3, float v4);

    // Count is the count of vecn not of individual floats
    void set_array_f1(const char* u_name, const float* v, uint32_t count);
    void set_array_f2(const char* u_name, const float* v, uint32_t count);
    void set_array_f3(const char* u_name, const float* v, uint32_t count);
    void set_array_f4(const char* u_name, const float* v, uint32_t count);

    void set_mat_f3(const char* u_name, const glm::mat3 mat);
    void set_mat_f4(const char* u_name, const glm::mat4 mat);

    void set_i1(const char* u_name, int v);
    void set_i2(const char* u_name, int v1, int v2);
    void set_i3(const char* u_name, int v1, int v2, int v3);
    void set_i4(const char* u_name, int v1, int v2, int v3, int v4);

    // Count is the count of vecn not of individual floats
    // TODO
    void set_array_i1(const char* u_name, const int* v, uint32_t count);
    void set_array_i2(const char* u_name, const int* v, uint32_t count);
    void set_array_i3(const char* u_name, const int* v, uint32_t count);
    void set_array_i4(const char* u_name, const int* v, uint32_t count);

    void set_u1(const char* u_name, uint32_t v);
    void set_u2(const char* u_name, uint32_t v1, uint32_t v2);
    void set_u3(const char* u_name, uint32_t v1, uint32_t v2, uint32_t v3);
    void set_u4(const char* u_name, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);

    // Count is the count of vecn not of individual floats
    // TODO
    void set_array_u1(const char* u_name, const uint32_t* v, uint32_t count);
    void set_array_u2(const char* u_name, const uint32_t* v, uint32_t count);
    void set_array_u3(const char* u_name, const uint32_t* v, uint32_t count);
    void set_array_u4(const char* u_name, const uint32_t* v, uint32_t count);
};
