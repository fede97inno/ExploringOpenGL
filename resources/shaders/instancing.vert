#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

// 4.
layout (location = 2) in mat4 inst_mvp; // we pass by a buffer that we update every draw call on 6 vertex

// mat4 it's like -> vec4 location 
// layout (location = 2) in vec4 inst_mvp; 
// layout (location = 3) in vec4 inst_mvp;
// layout (location = 4) in vec4 inst_mvp;
// layout (location = 5) in vec4 inst_mvp;


// 1-2.
//uniform mat4 mvp;

// 3.
//uniform mat4 mvp[4];

out flat int inst_id_out;
out vec2 vert_uv_out;

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

    // 4. next
    gl_Position = inst_mvp * vec4(vert_pos, 1.0f);
    inst_id_out = gl_InstanceID;
    vert_uv_out = vert_uv;
}