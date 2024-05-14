#include "OGLTexture.h"
#include "Common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint CreateTexture(const std::string& inFilePath)
{
    stbi_set_flip_vertically_on_load(true); //this flips y, impact everything that cames after, but if u do this u will do it only the first time
    
    int width, height, channels;
    unsigned char* data = stbi_load(inFilePath.c_str(), &width, &height, &channels, 0);

    DIE_ON_NULL(data, "Error Reading Image");

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

OGLTexture::OGLTexture(const std::string& inFilePath)
{
    id = CreateTexture(inFilePath);
}

OGLTexture::~OGLTexture()
{
    glDeleteTextures(1, &id);
}
void OGLTexture::Bind(GLuint inTextureSlot)
{
    glActiveTexture(inTextureSlot);   // Active the spot where to put the texture
    glBindTexture(GL_TEXTURE_2D, id);
}
