#version 430 core

in vec4 prov_color;

out vec4 out_color;


void main() {
    out_color = prov_color/255.0f;
}
