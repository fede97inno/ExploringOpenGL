#version 460 core
// Called for every vertex (less time than frag)

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

vec3 perspective(vec3 pos)
{
    // Fov - Aspect Ratio

    float aspect_ratio = 800.0f / 600.0f;   // Witdth / Height
    float vert_fov = 60.0f;
    float fov = tan(radians(vert_fov *0.5f));

    vec3 persp;
    persp.x = pos.x / (fov * aspect_ratio);
    persp.y = pos.y / fov;
    persp.z = pos.z;

    return persp;
}

void main()
{
    vec3 fixed_vert = perspective(vert_pos);

    gl_Position = vec4(fixed_vert, 1.0f);
    vert_uv_out = vert_uv;
}