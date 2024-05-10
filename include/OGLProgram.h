#pragma once
#include <string>
#include <glad/glad.h>

class OGLProgram
{
public:
    OGLProgram(const std::string& inVertShaderPath, const std::string& inFragShaderPath);
    ~OGLProgram();
    void Bind();
    const GLuint GetId() const;
private:
    GLuint programId;
};