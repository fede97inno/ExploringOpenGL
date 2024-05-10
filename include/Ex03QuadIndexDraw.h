#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"

class Ex03QuadIndexDraw
{
public:
    void Start();
    void Update(float inDeltaTime);
    void Destroy();
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    OGLProgram* program;
};