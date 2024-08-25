// framebuffer.cpp

#include "include/framebuffer.hpp"

#include <iostream>

#include "../../third-party/glad/include/glad/glad.h"

using namespace std;

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &frameBufferID);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &frameBufferID);
}

void FrameBuffer::BindTexture(unsigned int textureID) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "[Error] FrameBuffer: Framebuffer is incomplete." << endl;
        exit(-1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::GetFrameBufferID() const {
    return frameBufferID;
}
