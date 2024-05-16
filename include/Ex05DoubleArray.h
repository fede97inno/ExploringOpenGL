#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex05DoubleArray
{
public:
    void Start();
    void Update(float inDeltaTime);
    void Destroy();
private:
    GLuint vao;     // Vertex Array Object   -> store the assosiation between vertex data and the buffer that contains them
    GLuint vboPos;     // Vertex Buffer Object  -> buffer where are store datas on the vertices (positions, colors, normals)
    GLuint vboUvs;      // -> array for uvs, we create like this or if we want to be more dynamic we can use GLuint vbo[numbers of arrays needed];
    GLuint ebo;     // Element Buffer Object -> buffer that contains the index of the vertices for defining forms primitives
    OGLProgram* program;
    OGLTexture* smileTexture;
    OGLTexture* woodBoxTexture;
};