#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Ex08MovingLight
{
public:
    void Start();
    void Update(float inDeltaTime);
    void Destroy();
private:
    GLuint vao;     // Vertex Array Object   -> store the assosiation between vertex data and the buffer that contains them
    GLuint vbo;     // Vertex Buffer Object  -> buffer where are store datas on the vertices (positions, colors, normals)
    GLuint vaoLight;
    GLuint vboLight;
    OGLProgram* program;
    OGLProgram* programLight;
    OGLTexture* stormTexture;
    glm::mat4 view;
    glm::mat4 projection;
    size_t verticesCount;
};