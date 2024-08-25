// texture1D.hpp

#pragma once

#include "../../../third-party/glad/include/glad/glad.h"

class Texture1D {
    public:
        Texture1D(int width,
                  GLint warp,
                  GLint filter,
                  GLint internalFormat,
                  GLenum format,
                  GLenum type,
                  const char *data);

        ~Texture1D();

        void UpdateTexture(int width,
                           GLint warp,
                           GLint filter,
                           GLint internalFormat,
                           GLenum format,
                           GLenum type,
                           const char *data);

        int GetTextureWidth() const;
        unsigned int GetTextureID() const;

    private:
        int textureWidth;
        unsigned int textureID;
};
