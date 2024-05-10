#include "Ex04QuadColorDraw.h"
#include <string>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

void Ex04QuadColorDraw::Start() 
{
    program = new OGLProgram("resources/shaders/quadcolor.vert", "resources/shaders/quadcolor.frag");

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        // Positions       //Colors
        0.5, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-right
       -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-left
        0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top-right
       -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f // top-left
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
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);   // Static cause they dont change

    // 3. Link to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);
    
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_1);

    // 4. Create EBO (element buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);

    // 5. Set Viewport
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    program->Bind();

    GLint baseColorLocation = glGetUniformLocation(program->GetId(), "base_color");
    Color colorVar{0.5f, 0.5f, 0.5f, 1.0f};
    const GLfloat* colorVarPtr = reinterpret_cast<GLfloat*>(&colorVar);
    glUniform4fv(baseColorLocation, 1, colorVarPtr);
}

void Ex04QuadColorDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    static float elapsedTime = 0;   // Will prevent resets
    elapsedTime += inDeltaTime;
    float value = sinf(elapsedTime) * 0.5f + 0.5f;
    
    Color timedColor;
    timedColor.r = value;
    timedColor.g = value;
    timedColor.b = value;
    timedColor.a = 1.0f;
    const GLfloat* colorVarPtr = reinterpret_cast<GLfloat*>(&timedColor);

    float scale = 0.5f;
    glUniform1f(glGetUniformLocation(program->GetId(), "scale"), scale);
    
    glUniform4fv(glGetUniformLocation(program->GetId(), "base_color"), 1, colorVarPtr);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // (void*)0 -> index 0
}

void Ex04QuadColorDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    delete program;
}