#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D box_tex;

in flat int inst_id_out;
in vec2 vert_uv_out;

void main()
{
    /*
    vec4 color;

    if (inst_id_out % 2 == 0)
    {
        color = texture(smile_tex, vert_uv_out);
    }
    else 
    {
        color = texture(box_tex, vert_uv_out);
    }

    frag_color = color;
    */

    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 box_texel = texture(box_tex, vert_uv_out);

    frag_color = mix(smile_texel, box_texel, inst_id_out % 2);
}