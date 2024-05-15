#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex07CubePerspectiveDraw
{
public:
    void Start();
    void Update(float inDeltaTime);
    void Destroy();
private:
    GLuint vao;     // Vertex Array Object   -> store the assosiation between vertex data and the buffer that contains them
    GLuint vbo;     // Vertex Buffer Object  -> buffer where are store datas on the vertices (positions, colors, normals)
    OGLProgram* program;
    OGLTexture* woodBoxTexture;
};