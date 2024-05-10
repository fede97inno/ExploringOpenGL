#version 460 core

out vec4 frag_color;

in vec4 vert_color_out;
uniform vec4 base_color;

void main()
{
    frag_color = vert_color_out * base_color;
}