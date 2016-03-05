#version 330 core

in vec4 f_color;
out vec4 o_color;

in vec2 f_tex;
uniform sampler2D u_sprite_tex;

void main (void)
{
    //o_color = f_color;
    //o_color = f_color * texture(u_sprite_tex, f_tex);
    o_color = texture(u_sprite_tex, f_tex);
}
