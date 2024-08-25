// texture2D.hpp

#pragma once

#include "../../../third-party/glad/include/glad/glad.h"

class Texture2D {
    public:
        Texture2D(int width,
                  int height,
                  GLint warp,
                  GLint filter,
                  GLint internalFormat,
                  GLenum format,
                  GLenum type,
                  const char *data);

        ~Texture2D();

        void UpdateTexture(int width,
                           int height,
                           GLint warp,
                           GLint filter,
                           GLint internalFormat,
                           GLenum format,
                           GLenum type,
                           const char *data);

        int GetTextureWidth() const;
        int GetTextureHeight() const;
        unsigned int GetTextureID() const;

    private:
        int textureWidth, textureHeight;
        unsigned int textureID;
};
