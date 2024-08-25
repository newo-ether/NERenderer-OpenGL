// texture2D.cpp

#include "include/texture2D.hpp"

#include "../../third-party/glad/include/glad/glad.h"

Texture2D::Texture2D(int width,
                     int height,
                     GLint warp,
                     GLint filter,
                     GLint internalFormat,
                     GLenum format,
                     GLenum type,
                     const char *data) {

    glGenTextures(1, &textureID);
    UpdateTexture(width, height, warp, filter, internalFormat, format, type, data);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &textureID);
}

void Texture2D::UpdateTexture(int width,
                              int height,
                              GLint warp,
                              GLint filter,
                              GLint internalFormat,
                              GLenum format,
                              GLenum type,
                              const char *data) {
                              
    textureWidth = width;
    textureHeight = height;

    // Bind Texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Configure Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    // Copy Data to Texture
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

    // Unbind Texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture2D::GetTextureWidth() const {
    return textureWidth;
}

int Texture2D::GetTextureHeight() const {
    return textureHeight;
}

unsigned int Texture2D::GetTextureID() const {
    return textureID;
}
