// texture1D.cpp

#include "include/texture1D.hpp"

#include "../../third-party/glad/include/glad/glad.h"

Texture1D::Texture1D(int width,
                     GLint warp,
                     GLint filter,
                     GLint internalFormat,
                     GLenum format,
                     GLenum type,
                     const char *data) {

    glGenTextures(1, &textureID);
    UpdateTexture(width, warp, filter, internalFormat, format, type, data);
}

Texture1D::~Texture1D() {
    glDeleteTextures(1, &textureID);
}

void Texture1D::UpdateTexture(int width,
                              GLint warp,
                              GLint filter,
                              GLint internalFormat,
                              GLenum format,
                              GLenum type,
                              const char *data) {

    textureWidth = width;

    // Bind Texture
    glBindTexture(GL_TEXTURE_1D, textureID);

    // Configure Texture
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, warp);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, warp);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, filter);

    // Copy Data to Texture
    glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, format, type, data);

    // Unbind Texture
    glBindTexture(GL_TEXTURE_1D, 0);
}

int Texture1D::GetTextureWidth() const {
    return textureWidth;
}

unsigned int Texture1D::GetTextureID() const {
    return textureID;
}
