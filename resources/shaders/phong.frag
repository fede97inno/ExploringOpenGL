#version 460 core
// Called for every pixels (more times than vert)

out vec4 frag_color;

layout (binding = 0) uniform sampler2D storm_tex;

in vec2 vert_uv_out;
in vec3 vert_norm_out;
in vec3 vert_world_pos_out;
in vec3 vert_world_norm_out;

uniform vec3 point_light_pos;
uniform vec3 camera_pos;

void main()
{
    //frag_color = vec4(1, 0, 0, 1);
    //frag_color = vec4(vert_uv_out.x, vert_uv_out.y, 0, 1);
    //frag_color = texture(storm_tex, vert_uv_out);             // Base Texture

    // Debugging exemple:
    //frag_color = vec4(vert_norm_out, 1.0f);
    //frag_color = vec4(vert_world_norm_out, 1.0f);
    //return;

    vec4 texel = texture(storm_tex, vert_uv_out);
    vec3 pixel_color = texel.xyz;

    // Phong Implementation:

    // Ambient:
    float ambient_factor = 0.2f;
    vec3 ambient = pixel_color * ambient_factor;

    vec3 world_normal = normalize(vert_world_norm_out);

    // Diffuse:
    vec3 light_dir = normalize(point_light_pos - vert_world_pos_out);
    float lambert = max(dot(world_normal, light_dir), 0.0f);
    vec3 diffuse = pixel_color * lambert;

    // Specular
    vec3 eye_dir = normalize(camera_pos - vert_world_pos_out);  // all direction must be normalize
    vec3 refl_dir = reflect(-light_dir, world_normal);
    float specular_strenght = max(dot(eye_dir, refl_dir), 0.0f);
    float specular_factor = pow(specular_strenght, 40.0f);
    vec3 specular = vec3(1.0f,1.0f,1.0f) * specular_factor;

    // Phong:
    vec3 phong = vec3(0, 0, 0); 
    phong += ambient;
    phong += diffuse;
    phong += specular;

    // Light attenuation
    // Far from the light source light gets weaker
    // attenuation = constant + linear * distance + quadratic * distance^2 

    float k_c = 1.0f;           // Generally set to one cause stop division by 0
    float k_l = 0.045f;         // Values taken from OgreWiki
    float k_d = 0.0075f;

    float dist = length(point_light_pos - vert_world_pos_out);
    float attenuation = 1.0f / (k_c + k_l * dist + k_d * dist * dist);

    phong *= attenuation;

    frag_color = vec4(phong, 1.0f);
}