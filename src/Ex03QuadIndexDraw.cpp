#include "Ex03QuadIndexDraw.h"
#include <string>
#include <vector>
#include <iostream>
#include "OGLProgram.h"

void Ex03QuadIndexDraw::Start() 
{
    program = new OGLProgram("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        0.5, -0.5f, 0.0f,   // bottom-right
       -0.5f, -0.5f, 0.0f,  // bottom-left
        0.5f, 0.5f, 0.0f,   // top-right
       -0.5f, 0.5f, 0.0f,   // top-left
        };

    std::vector<uint32_t> indexes = { 
        0,1,2,  // Right Triangle
        2,3,1   // Left Triangle
    };

    // 1. Create VAO
    glGenVertexArrays(1, &vao);     // Generate vao
    glBindVertexArray(vao);         // Open vao

    // 2. Create VBO buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // loading data on gpu
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);

    // 3. Link to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);

    // 4. Create EBO (element buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);

    // 5. Set Viewport
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    program->Bind();

    // Draw Point
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    //glPointSize(8);

    //Draw Lines
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Ex03QuadIndexDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // (void*)0 -> index 0
}

void Ex03QuadIndexDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    delete program;
}