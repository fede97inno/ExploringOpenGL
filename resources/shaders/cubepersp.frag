#version 460 core
// Called for every pixels (more times than vert)
out vec4 frag_color;

in vec4 vert_color_out;
in vec2 vert_uv_out;

uniform vec4 base_color;
layout (binding = 1) uniform sampler2D wood_box_tex;

void main()
{
    vec4 wood_box_texel = texture(wood_box_tex, vert_uv_out);

    frag_color = wood_box_texel;
}