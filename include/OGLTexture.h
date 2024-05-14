#pragma once
#include <string>
#include <glad/glad.h>

class OGLTexture
{
public:
    OGLTexture(const std::string& inFilePath);
    ~OGLTexture();
    void Bind(GLuint inTextureSlot);
    const GLuint GetId() const { return id; }
private:
    GLuint id;
};