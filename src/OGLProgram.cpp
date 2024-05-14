#include "OGLProgram.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "Common.h"

static std::string ReadFile(const std::string& inPath)  // With static we can use the same name for differents functions
{
    std::ifstream inputStream(inPath, std::ios::ate);   // Open the file moving the cursor ate -> at the end 
    size_t fileSize = inputStream.tellg();              // .tellg -> Cursor Pos in bytes, it's the size cause the cursor has been moved at the end

    std::string text;
    text.resize(fileSize);

    inputStream.seekg(0,std::ios::beg);                 // Rewind cursor position
    inputStream.read(&text[0], fileSize);               // Read the whole file

    inputStream.close();

    return text;
}

static GLuint CreateShader(const std::string& inPath, GLuint inShaderType)
{
    std::string text = ReadFile(inPath);
    const char* shaderSource = text.c_str();

    GLuint shaderId = glCreateShader(inShaderType);
    glShaderSource(shaderId, 1, &shaderSource, NULL);

    glCompileShader(shaderId);

    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint maxLogLenght;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLogLenght);
        std::vector<GLchar> infoLog(maxLogLenght);

        glGetShaderInfoLog(shaderId, maxLogLenght, NULL, infoLog.data());

        std::string logString(infoLog.begin(), infoLog.end());
        std::cout << "Shader Compilation failure: " << logString << std::endl;
        throw std::runtime_error(logString);
    }

    return shaderId;
}

static GLuint CreateProgram(GLuint vertexId, GLuint fragmentId)
{
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLint maxLogLenght;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLogLenght);
        std::vector<GLchar> infoLog(maxLogLenght);

        glGetProgramInfoLog(programId, maxLogLenght, NULL, infoLog.data());

        std::string logString(infoLog.begin(), infoLog.end());
        std::cout << "Program linking failure: " << logString << std::endl;
        throw std::runtime_error(logString);
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    return programId;
}

OGLProgram::OGLProgram(const std::string& inVertShaderPath, const std::string& inFragShaderPath)
{
    //Create Shader vert - frag Read&Compile
    GLuint vertexShaderId = CreateShader(inVertShaderPath, GL_VERTEX_SHADER);          // Executed for every Vertex
    GLuint fragmentShaderId = CreateShader(inFragShaderPath, GL_FRAGMENT_SHADER);      // Executed for every pixel in the triangle

    //Create Program Linking
    programId = CreateProgram(vertexShaderId, fragmentShaderId);
}

OGLProgram::~OGLProgram()
{
    glDeleteProgram(programId);
}

void OGLProgram::Bind()
{
    glUseProgram(programId);
}

const GLuint OGLProgram::GetId() const
{
    return programId;
}

void OGLProgram::SetUniform(const std::string& inName, const Color& inColor)
{
    glUniform4fv(glGetUniformLocation(GetId(), inName.c_str()), 1, reinterpret_cast<const GLfloat*>(&inColor));
}

void OGLProgram::SetUniform(const std::string& inName, const float inValue)
{
    glUniform1f(glGetUniformLocation(GetId(), inName.c_str()), inValue);
}