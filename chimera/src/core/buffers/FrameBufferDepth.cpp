#include "chimera/core/buffers/FrameBufferDepth.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

FrameBufferDepth::FrameBufferDepth(const uint16_t& width, const uint16_t& height) : width(width), height(height) {

    TexParam td(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT);
    texture = new Texture("", width, height, td);

    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenFramebuffers(1, &framBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getTextureID(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferDepth::~FrameBufferDepth() { glDeleteFramebuffers(1, &framBufferID); }

void FrameBufferDepth::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);
    glViewport(0, 0, width, height);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void FrameBufferDepth::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBufferDepth::clear() {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_DEPTH_BUFFER_BIT);
}

} // namespace Chimera