#version 460 core
// Called for every vertex (less time than frag)

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

uniform float rotation;

vec3 perspective(vec3 pos)
{
    // Fov - Aspect Ratio

    float aspect_ratio = 800.0f / 600.0f;   // Witdth / Height
    float vert_fov = 60.0f;
    float fov = tan(radians(vert_fov *0.5f));

    float near = 0.01f;
    float far = 100.0f;
    float range = far - near;

    // Fix perspective
    vec3 persp;
    persp.x = pos.x / -pos.z;
    persp.y = pos.y / -pos.z;
    persp.z = ((-pos.z - near) / range) * 2.0f - 1.0f;

    // Fix camera angle and aspect ratio
    persp.x = persp.x / (fov * aspect_ratio);
    persp.y = persp.y / fov;

    return persp;
}

vec3 rotate_on_y(vec3 pos, float degrees)
{
    float rads = radians(degrees);
    vec3 result;

    // Formula for bidimensional rotations
    // x' = x * cos(a) - y * sin(a)
    // y' = x * sin(a) + y * cos(a)

    result.x = cos(rads) * pos.x - sin(rads) * pos.z; 
    result.z = sin(rads) * pos.x + cos(rads) * pos.z;

    result.y = pos.y;       // it's the same cause we rotate on this axe

    return result;
}

void main()
{
    vec3 fixed_vert = vert_pos;
    fixed_vert = rotate_on_y(fixed_vert, rotation);
    fixed_vert.z -= 4.f;    // Translate
    fixed_vert = perspective(fixed_vert);


    gl_Position = vec4(fixed_vert, 1.0f);
    vert_uv_out = vert_uv;
}