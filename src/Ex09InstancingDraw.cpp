#include "Ex09InstancingDraw.h"
#include <string>
#include <iostream>
#include "OGLProgram.h"
#include <format>

// Ex09 Drawing N quads on screen
void Ex09InstancingDraw::Start() 
{
    program = new OGLProgram("resources/shaders/instancing.vert", "resources/shaders/instancing.frag");

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

    // Camera infos
    glm::vec3 position = glm::vec3(0, 0, 8.0f);     // World Position
    glm::vec3 direction = glm::vec3(0, 0, -1);      // Direction, look to negative Z
    glm::vec3 cameraUp = glm::vec3(0, 1, 0);        // Up of the camera, we take the poitive y normalized = 1 
    float fovY = 60.0f;                             // Fov Y
    float aspectRatio = 800.0f/ 600.0f;             // Aspect Ratio
    float zNear = 0.01f;                            // Near Plane
    float zFar = 100.0f;                            // Far Plane

    view = glm::lookAt(position, position + direction, cameraUp);   // View Matrix -> lookAt(postion, where i'm looking, up)
    projection = glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar);

    quads.resize(4);

    for (int index = 0; index < quads.size(); ++index)
    {    
        quads[index].Position = glm::vec3(-3 + index, -3 + index, 0.0f);
    }
}

void Ex09InstancingDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);

    /*
    // -> this thing here will call three copies
    for (int index = 0; index < 4; ++index)
    {    
        // Model matrix = Translate * Rotate * Scale
        glm::mat4 model = glm::mat4(1.0f);                                      // Identity Matrix
        model = glm::translate(model, glm::vec3(-3.0f + index, -3.0f + index, 0.0f));            // Apply Translation(origin matrix, translation vector3)
        model = glm::scale(model, glm::vec3(index + 0.25f, index + 0.25f, index + 0.25f));  
        glm::mat4 mvp = projection * view * model;
        program->SetUniform("mvp", mvp);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);                    
    }
    */

    // glm::mat4 model = glm::mat4(1.0f);    
    // glm::mat4 mvp = projection * view * model;
    // program->SetUniform("mvp", mvp);
    // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);    // Single draw call

    for (int index = 0; index < quads.size(); ++index)
    {
        glm::mat4 model = glm::mat4(1.0f);                                      // Identity Matrix
        model = glm::translate(model, quads[index].Position);            // Apply Translation(origin matrix, translation vector3)
        glm::mat4 mvp = projection * view * model;
        std::string indexedUniform = std::format("mvp[{}]", index);

        program->SetUniform(indexedUniform, mvp);
    }
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, quads.size());     // Single Draw call
}

void Ex09InstancingDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    delete program;
}