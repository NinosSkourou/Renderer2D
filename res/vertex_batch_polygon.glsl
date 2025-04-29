#version 430 core

in vec2 in_position;
in mat4 in_model_mat;
in vec4 in_color;
out vec4 prov_color;

uniform mat4 u_projection_mat;
uniform mat4 u_view_mat;

void main() {
    prov_color = in_color;
    
    mat4 MVP = u_projection_mat*u_view_mat*in_model_mat;

    vec4 new_pos = MVP * (vec4(in_position, 0.0f, 1.0f));
    gl_Position = new_pos;
}
