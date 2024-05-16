#include "Ex05DoubleArray.h"
#include <string>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Common.h"

void Ex05DoubleArray::Start() 
{
    program = new OGLProgram("resources/shaders/quadtexture.vert", "resources/shaders/quadtexture.frag");

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        // Positions       // UVs
        0.5, -0.5f,0.0f,    // bottom-right
       -0.5f,-0.5f,0.0f,   // bottom-left
        0.5f, 0.5f,0.0f,    // top-right
       -0.5f, 0.5f,0.0f    // top-left
        };

    std::vector<float> uvs =
    {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f 
    };

    std::vector<uint32_t> indexes = { 
        0,1,2,  // Right Triangle
        2,3,1   // Left Triangle
    };

    // 1. Create VAO
    glGenVertexArrays(1, &vao);     // Generate vao
    glBindVertexArray(vao);         // Open vao

    // 2. Create VBOs buffers
    glGenBuffers(1, &vboPos);
    glGenBuffers(1, &vboUvs);
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);

    // loading data on gpu
    int verticesDataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize, vertices.data(), GL_STATIC_DRAW);   // Static cause they dont change

    // 3. Link to Vertex Shader -> these are the pins location_0 in shader files
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboUvs);
    int uvsDataSize = uvs.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, uvsDataSize, uvs.data(), GL_STATIC_DRAW);

    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_1);

    // 4. Create EBO (element buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);

    // 5. Set Viewport
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    program->Bind();

    program->SetUniform("base_color", {0.5f, 0.5f, 0.5f, 1.0f});

    // 6. Create Texture
    smileTexture = new OGLTexture("resources/textures/smile.png");
    woodBoxTexture = new OGLTexture("resources/textures/wood-box.jpg");
    
    smileTexture->Bind(GL_TEXTURE0);
    woodBoxTexture->Bind(GL_TEXTURE1);

    glUniform1i(glGetUniformLocation(program->GetId(), "wood_box_tex"), 1);     // Set the layout to 1

    // 7. Enable Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 1 - src
}

void Ex05DoubleArray::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // (void*)0 -> index 0
}

void Ex05DoubleArray::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vboPos);
    glDeleteBuffers(1, &vboUvs);
    glDeleteBuffers(1, &ebo);

    delete smileTexture;
    delete woodBoxTexture;
    delete program;
}

