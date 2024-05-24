#pragma once
#include <string>
#include "Common.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class OGLProgram
{
public:
    OGLProgram(const std::string& inVertShaderPath, const std::string& inFragShaderPath);
    ~OGLProgram();
    void Bind();
    const GLuint GetId() const;
    void SetUniform(const std::string& inName, const Color& inColor);
    void SetUniform(const std::string& inName, float inValue);
    void SetUniform(const std::string& inName, const glm::mat4& inMatrix);
    void SetUniform(const std::string& inName, const glm::vec3& inVec3);
private:
    GLuint programId;
};