#include "chimera/core/buffers/FrameBuffer.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

FrameBuffer::FrameBuffer(const uint16_t& width, const uint16_t& height) : width(width), height(height) {

    // refs:
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    // https://github.com/andersonfreitas/opengl-tutorial-org/blob/master/tutorial14_render_to_texture/tutorial14.cpp

    // pass 1 cria framebuffer // FramebufferName => fbo
    glGenFramebuffers(1, &framBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);

    // pass 2 create textura that will be used as a color buffer // renderedTexture => texture
    TextureParameters params(TextureFormat::RGBA, TextureFormat::RGBA, TextureFilter::LINEAR, TextureWrap::CLAMP,
                             TextureDataType::UNSIGNED_BYTE);
    texture = new Texture("", width, height, params);

    // pass 3  // The depth buffer // depthrenderbuffer => fb_depth
    glGenRenderbuffers(1, &depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

    // Pass 4 //Set "renderedTexture" as our colour attachement #0
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->getTextureID(), 0);

    // Set the list of draw buffers. // ??????
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw Exception(std::string("Falha em instanciar o Frame Buffer"));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &framBufferID);
    glDeleteRenderbuffers(1, &depthBufferID);
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);
    glViewport(0, 0, width, height);
}

void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::clear() {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
} // namespace Chimera