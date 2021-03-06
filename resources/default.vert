#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_col;
layout(location = 2) in vec2 a_tex;

uniform mat4 u_mvp_mat;
uniform mat4 u_mod_mat;
uniform mat4 u_view_mat;
uniform mat4 u_proj_mat;

out vec4 f_color;
out vec2 f_tex;

void main() {
    gl_Position =  u_proj_mat * u_view_mat * u_mod_mat * vec4(a_pos, 1.0);
    f_tex = a_tex;
    f_color = a_col;
}
