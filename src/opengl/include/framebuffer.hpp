// framebuffer.hpp

#pragma once

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void BindTexture(unsigned int textureID);
        unsigned int GetFrameBufferID() const;

    private:
        unsigned int frameBufferID;
};
