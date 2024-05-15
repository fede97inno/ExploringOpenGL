#include "Ex07CubePerspectiveDraw.h"
#include <string>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Common.h"

void Ex07CubePerspectiveDraw::Start() 
{
    program = new OGLProgram("resources/shaders/cubepersp.vert", "resources/shaders/cubepersp.frag");

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        // Positions   // UVs
        /*
  pos    uv        pos    uv
(-1,1,1)(0,1)    (1,1,1)(1,1)
        o----------o
        |          |
        |          |
        |          |
        |          |
        o----------o
(-1,-1,1)(0,0)    (1,-1,1)(1,0)
   pos    uv         pos   uv
        */
        // Front Face
        1, 1, 1,        1, 1,       // Top - Right
       -1, 1, 1,        0, 1,       // Top - Left
       -1,-1, 1,        0, 0,       // Bottom - Left

        1, 1, 1,        1, 1,       // Top - Right
       -1,-1, 1,        0, 0,       // Bottom - Left
        1,-1, 1,        1, 0,       // Bottom - Right

        // Right Face
        1, 1,-1,        1, 1,       // Top - Right
        1, 1, 1,        0, 1,       // Top - Left
        1,-1, 1,        0, 0,       // Bottom - Left

        1, 1,-1,        1, 1,       // Top - Right
        1,-1, 1,        0, 0,       // Bottom - Left
        1,-1,-1,       1, 0,       // Bottom - Right

        // Left Face
       -1, 1, 1,        1, 1,       // Top - Right
       -1, 1,-1,        0, 1,       // Top - Left
       -1,-1,-1,        0, 0,       // Bottom - Left

       -1, 1, 1,        1, 1,       // Top - Right
       -1,-1,-1,        0, 0,       // Bottom - Left
       -1,-1, 1,        1, 0,       // Bottom - Right

        // Back Face
        -1, 1,-1,       1, 1,
         1, 1,-1,       0, 1,
         1,-1,-1,       0, 0,

        -1, 1,-1,       1, 1,
         1,-1,-1,       0, 0,
        -1,-1,-1,       1, 0,

        // Top Face
         1, 1,-1,       1, 1,
        -1, 1,-1,       0, 1,
        -1, 1, 1,       0, 0,

         1, 1,-1,       1, 1,
        -1, 1, 1,       0, 0,
         1, 1, 1,       1, 0,

        // Bottom Face
         1,-1, 1,       1, 1,
        -1,-1, 1,       0, 1,
        -1,-1,-1,       0, 0,

         1,-1, 1,       1, 1,
        -1,-1,-1,       0, 0,
         1,-1,-1,       1, 0,
        };

    // 1. Create VAO
    glGenVertexArrays(1, &vao);     // Generate vao
    glBindVertexArray(vao);         // Open vao

    // 2. Create VBO buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // loading data on gpu
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);   // Static cause they dont change

    // 3. Link to Vertex Shader -> these are the pins location_0 in shader files
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);
    
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_1);


    // 4. Set Viewport
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    program->Bind();

    // 5. Create Texture
    woodBoxTexture = new OGLTexture("resources/textures/wood-box.jpg");

    woodBoxTexture->Bind(GL_TEXTURE1);

    glUniform1i(glGetUniformLocation(program->GetId(), "wood_box_tex"), 1);     // Set the layout to 1

    // 6. Enable Alpha Blending -> useless in this case there's no use for this
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 1 - src

    // 7. Enable DepthTesting
    glEnable(GL_DEPTH_TEST);    // draw first the near faces

    // 8. Enable FaceCulling
    glEnable(GL_CULL_FACE);     // draw only the visible faces
    glCullFace(GL_BACK);        // default
    glFrontFace(GL_CCW);        // GL_CW set clock wise instead of counter clock wise -> glFrontFace(GL_CCW); is the default
}

void Ex07CubePerspectiveDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float elapsedTime = 0;
    elapsedTime += inDeltaTime;

    program->SetUniform("rotation", 20.0f * elapsedTime);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Ex07CubePerspectiveDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    delete woodBoxTexture;
    delete program;
}

