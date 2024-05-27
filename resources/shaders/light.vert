#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vert_pos, 1.0f);
}