#include "Ex09InstancingDraw.h"
#include <string>
#include <iostream>
#include <format>

// Ex09 Drawing N quads on screen
void Ex09InstancingDraw::Start() 
{
    program = new OGLProgram("resources/shaders/instancing.vert", "resources/shaders/instancing.frag");


    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        0.5f,-0.5f, 0.0f,   1,0,    // bottom-right
       -0.5f,-0.5f, 0.0f,   0,0,    // bottom-left
        0.5f, 0.5f, 0.0f,   1,1,    // top-right
       -0.5f, 0.5f, 0.0f,   0,1     // top-left
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
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);

    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(location_1);

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
        auto& quad = quads[index];

        quad.Position = glm::vec3(-3 + index, -3 + index, 0.0f);
        quad.RotationSpeed = 20.0f * index;
    }

    mvpData.resize(quads.size());

    // Create MVP buffer
    glGenBuffers(1, &mvpVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mvpVbo);
    glBufferData(GL_ARRAY_BUFFER, mvpData.size() * sizeof(glm::mat4), nullptr, GL_STREAM_DRAW);   // stream cause we update datas, this is like a ctor for buffer

    GLuint location_mvp = 2;
    glVertexAttribPointer(location_mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);      // with pin mat4 are not atomic datas like vec3, mat4 uses 4 location, one line for location
    glEnableVertexAttribArray(location_mvp);
    glVertexAttribDivisor(location_mvp, 1);     // update Every drawcall not every vertex
    
    location_mvp++;
    glVertexAttribPointer(location_mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(location_mvp);
    glVertexAttribDivisor(location_mvp, 1);
    
    location_mvp++;
    glVertexAttribPointer(location_mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(location_mvp);
    glVertexAttribDivisor(location_mvp, 1);
    
    location_mvp++;
    glVertexAttribPointer(location_mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(location_mvp);
    glVertexAttribDivisor(location_mvp, 1);

    smileTexture = new OGLTexture("resources/textures/smile.png");
    boxTexture = new OGLTexture("resources/textures/wood-box.jpg");

    smileTexture->Bind(GL_TEXTURE0);
    boxTexture->Bind(GL_TEXTURE1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH);
    glDepthFunc(GL_LESS);
}

void Ex09InstancingDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* 1.
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

    // 2.
    // glm::mat4 model = glm::mat4(1.0f);    
    // glm::mat4 mvp = projection * view * model;
    // program->SetUniform("mvp", mvp);
    // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);    // Single draw call

    // 3.
    // for (int index = 0; index < quads.size(); ++index)
    // {
    //     glm::mat4 model = glm::mat4(1.0f);                                      // Identity Matrix
    //     model = glm::translate(model, quads[index].Position);            // Apply Translation(origin matrix, translation vector3)
    //     glm::mat4 mvp = projection * view * model;
    //     std::string indexedUniform = std::format("mvp[{}]", index);

    //     program->SetUniform(indexedUniform, mvp);
    // }
    // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, quads.size());     // Single Draw call

    // 4.
    static float elapsedTime = 0.0f;
    elapsedTime += inDeltaTime;

    for (int index = 0; index < quads.size(); ++index)
    {
        auto& quad = quads[index];
        glm::mat4 model = glm::mat4(1.0f);                               // Identity Matrix
        model = glm::translate(model, quad.Position);            // Apply Translation(origin matrix, translation vector3)
        model = glm::rotate(model, glm::radians(quad.RotationSpeed * elapsedTime), glm::vec3(0, 0, 1));

        glm::mat4 mvp = projection * view * model;

        mvpData[index] = mvp;

    }
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, mvpData.size() * sizeof(glm::mat4), mvpData.data());    // fill an initialized buffer, it's like a set

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, quads.size());
}

void Ex09InstancingDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    delete smileTexture;
    delete boxTexture;
    delete program;
}