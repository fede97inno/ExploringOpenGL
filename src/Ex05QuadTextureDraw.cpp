#include "Ex05QuadTextureDraw.h"
#include <string>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

GLuint CreateTexture(const std::string& inFilePath)
{
    stbi_set_flip_vertically_on_load(true); //this flips y, impact everything that cames after, but if u do this u will do it only the first time
    
    int width, height, channels;
    unsigned char* data = stbi_load(inFilePath.c_str(), &width, &height, &channels, 0);

    if (data == nullptr)
    {
        std::cout << "Error Reading Image" << std::endl;
        throw std::runtime_error("Error Reading Image");
    }

    GLenum format = channels == 3 ? GL_RGB : GL_RGBA;

    GLuint textureId;
    glGenTextures(1, &textureId);               // Num of texture, textureId(to fill)
    glBindTexture(GL_TEXTURE_2D, textureId);    // type of texture, textureId

    // Load Data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // x y <-> u v <-> s t
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);     // When the texture became smaller
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);    // When the texture became bigger

    // MipMapping -> optional
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}

void Ex05QuadTextureDraw::Start() 
{
    program = new OGLProgram("resources/shaders/quadtexture.vert", "resources/shaders/quadtexture.frag");

    // Counter-clock wise -> by now data is in NDC space
    std::vector<float> vertices = { 
        // Positions       // UVs
        0.5, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom-right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom-left
        0.5f, 0.5f, 0.0f,  1.0f, 1.0f,  // top-right
       -0.5f, 0.5f, 0.0f,  0.0f, 1.0f   // top-left
        };

    std::vector<uint32_t> indexes = { 
        0,1,2,  // Right Triangle
        2,3,1   // Left Triangle
    };

    // 1. Create VAO
    glGenVertexArrays(1, &vao);     // Generate vao
    glBindVertexArray(vao);         // Open vao

    // 2. Create VBO buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // loading data on gpu
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);   // Static cause they dont change

    // 3. Link to Vertex Shader -> these are the pins location_0 in shader files
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_0);
    
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // location in triangle.vert, how many elements, type of the elements, if u want to normalize your elements, when the row finish, where to begin
    glEnableVertexAttribArray(location_1);

    // 4. Create EBO (element buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);

    // 5. Set Viewport
    glViewport(0, 0, 800, 600); 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    program->Bind();

    GLint baseColorLocation = glGetUniformLocation(program->GetId(), "base_color");
    Color colorVar{0.5f, 0.5f, 0.5f, 1.0f};
    const GLfloat* colorVarPtr = reinterpret_cast<GLfloat*>(&colorVar);
    glUniform4fv(baseColorLocation, 1, colorVarPtr);

    // 6. Create Texture
    GLuint smileTextureId = CreateTexture("resources/textures/smile.png");
    GLuint woodTextureId = CreateTexture("resources/textures/wood-box.jpg");
    
    glActiveTexture(GL_TEXTURE0);   // Active the spot where to put the texture
    glBindTexture(GL_TEXTURE_2D, smileTextureId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, woodTextureId);

    glUniform1i(glGetUniformLocation(program->GetId(), "wood_box_tex"), 1);     // Set the layout to 1

    // 7. Enable Alpha Blending
    glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 1 - src

}

void Ex05QuadTextureDraw::Update(float inDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // (void*)0 -> index 0
}

void Ex05QuadTextureDraw::Destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    delete program;
}

