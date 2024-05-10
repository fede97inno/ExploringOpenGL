#include "Ex02QuadDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

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

void Ex02QuadDraw::Start() 
{
    //Create Shader vert - frag Read&Compile
    GLuint vertexShaderId = CreateShader("resources/shaders/triangle.vert", GL_VERTEX_SHADER);          // Executed for every Vertex
    GLuint fragmentShaderId = CreateShader("resources/shaders/triangle.frag", GL_FRAGMENT_SHADER);      // Executed for every pixel in the triangle

    //Create Program Linking
    programId = CreateProgram(vertexShaderId, fragmentShaderId);

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        // Right Triangle
        0.5, -0.5f, 0.0f,   // bottom-right
       -0.5f, -0.5f, 0.0f,  // bottom-left
        0.5f, 0.5f, 0.0f,   // top-right

        // Left Triangle
        0.5f, 0.5f, 0.0f,   // top-right
       -0.5f, 0.5f, 0.0f,   // top-left
       -0.5f, -0.5f, 0.0f   // bottom-left
        };

    // 1. Create VAO
    glGenVertexArrays(1, &vao);     // Generate vao
    glBindVertexArray(vao);         // Open vao

    // 2. Create VBO buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // loading data on gpu
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);

    // 3. Link to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);

    // 4. Set Vertex
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glUseProgram(programId);    // Set the current pipeline

    // Draw Point
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    //glPointSize(8);

    //Draw Lines
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Ex02QuadDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Ex02QuadDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(programId);
}