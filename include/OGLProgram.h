#pragma once
#include <string>
#include "Common.h"
#include <glad/glad.h>

class OGLProgram
{
public:
    OGLProgram(const std::string& inVertShaderPath, const std::string& inFragShaderPath);
    ~OGLProgram();
    void Bind();
    const GLuint GetId() const;
    void SetUniform(const std::string& inName, const Color& inColor);
    void SetUniform(const std::string& inName, float inValue);
private:
    GLuint programId;
};