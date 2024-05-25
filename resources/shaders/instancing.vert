#version 460 core

layout (location = 0) in vec3 vert_pos;
// 4.
layout (location = 1) in mat4 inst_mvp; // we pass by a buffer that we update every draw call on 6 vertex

// 1-2.
//uniform mat4 mvp;

// 3.
uniform mat4 mvp[4];

void main()
{
    // 1.
    //gl_Position = mvp * vec4(vert_pos, 1.0f);
    
    // 2.
    // Every 6 vertex increment gl_InstanceID
    //float offset = gl_InstanceID / 2.0f;   // 0 to 3 i tell him to draw for four times 
    //gl_Position = mvp * vec4(vert_pos + offset, 1.0f);
    
    // 3.
    //gl_Position = mvp[gl_InstanceID] * vec4(vert_pos, 1.0f);

    // 4.
    gl_Position = inst_mvp * vec4(vert_pos, 1.0f);
}