#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_color;

out vec4 vert_color_out;

/*layout (location = 0)*/ uniform float scale;      // Even unifroms has layout location
//layout (location = 1) uniform float list[2];    // This occupy 2 space so the next one will be in location 3
void main()
{
    vec3 vert_pos_scaled = vert_pos * scale;

    //gl_Position = vec4(vert_pos, 1.0f);
    gl_Position = vec4(vert_pos_scaled, 1.0f);
    vert_color_out = vec4(vert_color, 1.0f);
}