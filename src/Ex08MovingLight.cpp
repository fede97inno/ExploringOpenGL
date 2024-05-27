#include "Ex08MovingLight.h"
#include <string>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Common.h"
#include "obj_parser.h"


void Ex08MovingLight::Start() 
{
    #pragma region Stormtrooper
    program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/phong.frag");

    obj_t* mesh = obj_parse("resources/models/stormtrooper.obj");

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices;

    for (int i = 0; i < mesh->triangle_count; i++)
    {
        obj_triangle_t& t = mesh->triangles[i];      // I want to take a ref cause i want to avoid the copy

        // Vertex 1
        vertices.push_back(t.v1.position.x);
        vertices.push_back(t.v1.position.y);
        vertices.push_back(t.v1.position.z);
        vertices.push_back(t.v1.uv.x);
        vertices.push_back(t.v1.uv.y);
        vertices.push_back(t.v1.normal.x);
        vertices.push_back(t.v1.normal.y);
        vertices.push_back(t.v1.normal.z);

        // Vertex 2
        vertices.push_back(t.v2.position.x);
        vertices.push_back(t.v2.position.y);
        vertices.push_back(t.v2.position.z);
        vertices.push_back(t.v2.uv.x);
        vertices.push_back(t.v2.uv.y);
        vertices.push_back(t.v2.normal.x);
        vertices.push_back(t.v2.normal.y);
        vertices.push_back(t.v2.normal.z);
        
        // Vertex 3
        vertices.push_back(t.v3.position.x);
        vertices.push_back(t.v3.position.y);
        vertices.push_back(t.v3.position.z);
        vertices.push_back(t.v3.uv.x);
        vertices.push_back(t.v3.uv.y);
        vertices.push_back(t.v3.normal.x);
        vertices.push_back(t.v3.normal.y);
        vertices.push_back(t.v3.normal.z);
    }
    
    obj_parse_destroy(mesh);

    verticesCount = mesh->triangle_count * 3;

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
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);
    
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_1);

    GLuint location_2 = 2;
    glVertexAttribPointer(location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(location_2);
    // 4. Set Viewport
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // 5. Create Texture
    stormTexture = new OGLTexture("resources/models/stormtrooper.png");
    stormTexture->Bind(GL_TEXTURE0);

    // 6. Enable DepthTesting
    glEnable(GL_DEPTH_TEST);    // draw first the near faces

    // 7. Enable FaceCulling
    glEnable(GL_CULL_FACE);     // draw only the visible faces
    glCullFace(GL_BACK);        // default
    glFrontFace(GL_CCW);        // GL_CW set clock wise instead of counter clock wise -> glFrontFace(GL_CCW); is the default

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
    program->SetUniform("camera_pos", position);

    // Light

    #pragma endregion
    
    programLight = new OGLProgram("resources/shaders/light.vert", "resources/shaders/light.frag");

    std::vector<float> cubeVertices = { 
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
    glGenVertexArrays(1, &vaoLight);
    glBindVertexArray(vaoLight);

    // 2. Create VBO buffer
    glGenBuffers(1, &vboLight);
    glBindBuffer(GL_ARRAY_BUFFER, vboLight);

    // loading data on gpu
    int lightDataSize = cubeVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, lightDataSize, cubeVertices.data(), GL_STATIC_DRAW);   // Static cause they dont change

    // 3. Link to Vertex Shader -> these are the pins location_0 in shader files
    GLuint lightLocation_0 = 0;
    glVertexAttribPointer(lightLocation_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);
    
    GLuint lightLocation_1 = 1;
    glVertexAttribPointer(lightLocation_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_1);
}

void Ex08MovingLight::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 stormPosition = glm::vec3(0.0f, -4.0f, 0.0f);
    static float elapsedTime = 0;
    elapsedTime += inDeltaTime;

    float angle = 20.0f * elapsedTime;
    programLight->Bind();
    glm::vec3 lightPosition = glm::vec3(4.0f, 0.0f, 0.0f);

    glBindVertexArray(vaoLight);
    
    glm::mat4 rotateAround = glm::mat4(1.0f);                   // translate * rot * translate with the center as the object                     
    rotateAround = glm::translate(rotateAround, stormPosition);
    rotateAround = glm::rotate(rotateAround, glm::radians(angle), glm::vec3(0, 1, 0));    
    rotateAround = glm::translate(rotateAround, -stormPosition);            
    
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPosition);
    lightModel = glm::scale(lightModel, glm::vec3(0.25f));
    //modelLight = glm::scale(modelLight, glm::vec3(2.0f));                             
    
    glm::mat4 mvpLight = projection * view * rotateAround * lightModel;
    
    programLight->SetUniform("mvp", mvpLight);

    glDrawArrays(GL_TRIANGLES, 0, 36);
 
    program->Bind();
    glBindVertexArray(vao);


    // Model matrix = Translate * Rotate * Scale
    glm::mat4 model = glm::mat4(1.0f);                                      // Identity Matrix
    model = glm::translate(model, stormPosition);            // Apply Translation(origin matrix, translation vector3)
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));    // Apply Rotation(origin matrix, angle, rotation axys)
    model = glm::scale(model, glm::vec3(2.0f));                             // Apply Scaling(origin matrix, scaling vector3)

    glm::mat4 mvp = projection * view * model;
    glm::vec3 pointLightPos = glm::vec3(4, 0, 0);

    program->SetUniform("mvp", mvp);
    program->SetUniform("model", model);
    program->SetUniform("point_light_pos", pointLightPos);
    
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);


}

void Ex08MovingLight::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vaoLight);
    glDeleteBuffers(1, &vboLight);

    delete stormTexture;
    delete program;
    delete programLight;
}

