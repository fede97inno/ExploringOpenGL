#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

struct QuadInstance
{
    glm::vec3 Position;
    float RotationSpeed;
};

class Ex09InstancingDraw
{
public:
    void Start();
    void Update(float inDeltaTime);
    void Destroy();
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint mvpVbo;
    OGLProgram* program;
    glm::mat4 view;
    glm::mat4 projection;
    std::vector<QuadInstance> quads;
    //4.
    std::vector<glm::mat4> mvpData;

    OGLTexture* smileTexture;
    OGLTexture* boxTexture;
};