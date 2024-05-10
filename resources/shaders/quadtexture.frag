#version 460 core
// Called for every pixels (more times than vert)
out vec4 frag_color;

in vec4 vert_color_out;
in vec2 vert_uv_out;

uniform vec4 base_color;
layout (binding = 0) uniform sampler2D smile_tex;       // -> correspond to tge GL_TEXTURE0 -> binding 0 is always done
layout (binding = 1) uniform sampler2D wood_box_tex;

void main()
{
    vec4 smile_texel = texture(smile_tex, vert_uv_out);     // texel is the smallest texture unit, in our case is equal to pixel
    vec4 wood_box_texel = texture(wood_box_tex, vert_uv_out);

    frag_color = mix(smile_texel, wood_box_texel, 0.5f);    // blend two text, the last is the percentage of each
}