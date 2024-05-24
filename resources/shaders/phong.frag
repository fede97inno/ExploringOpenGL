#version 460 core
// Called for every pixels (more times than vert)

out vec4 frag_color;

layout (binding = 0) uniform sampler2D storm_tex;

in vec2 vert_uv_out;
in vec3 vert_norm_out;

uniform vec3 point_light_pos;

void main()
{
    //frag_color = vec4(1, 0, 0, 1);
    //frag_color = vec4(vert_uv_out.x, vert_uv_out.y, 0, 1);
    //frag_color = texture(storm_tex, vert_uv_out);             // Base Texture

    vec4 texel = texture(storm_tex, vert_uv_out);

    // Phong Implementation:

    // Ambient:
    float ambient_factor = 0.2f;
    vec3 ambient = texel.xyz * ambient_factor;

    // Diffuse:
    // obtain light direction = point_light_pos - vert_world_pos
    // dot(light_direction, vert_norm) clamped in range [0,1]

    // Specular

    // Phong:
    vec3 phong = vec3(0, 0, 0); 
    phong += ambient;

    frag_color = vec4(phong, 1.0f);
}