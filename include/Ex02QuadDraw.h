#pragma once
#include "glad/glad.h"

class Ex02QuadDraw
{
public:
    void Start();
    void Update(float inDeltaTime);
    void Destroy();
private:
    GLuint vao;
    GLuint vbo;
    GLuint programId;
};