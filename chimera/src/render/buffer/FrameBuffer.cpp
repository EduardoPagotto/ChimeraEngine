#include "chimera/render/buffer/FrameBuffer.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/render/OpenGLDefs.hpp"

namespace Chimera {

namespace Aux {

static bool isDepthFormat(TexFormat format) {
    switch (format) {
        case TexFormat::DEPTH_COMPONENT:
        case TexFormat::DEPTH24STENCIL8:
            return true;
        default:
            break;
    }

    return false;
}

} // namespace Aux

static const uint32_t maxFrameBufferSize = 8192;

FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec) : spec(spec), framBufferID(0), rbo(0) {

    Aux::textureParameterSetUndefined(rboSpec);
    Aux::textureParameterSetUndefined(depthTexSpec);

    for (const TexParam& texParm : spec.attachments) {

        if (!Aux::isDepthFormat(texParm.format))
            colorTexSpecs.emplace_back(texParm); // color only
        else {
            if (texParm.filter != TexFilter::NONE) { // if has filter parameters them is a texture
                depthTexSpec = texParm;              // depth texture
            } else                                   // else
                rboSpec = texParm;                   // is a rbo
        }
    }

    this->invalidade();
}

FrameBuffer::~FrameBuffer() { this->destroy(); }

void FrameBuffer::destroy() {

    if (framBufferID) {
        glDeleteFramebuffers(1, &framBufferID);
        framBufferID = 0;

        if (!colorAttachments.empty()) {
            for (size_t i = 0; i < colorAttachments.size(); i++) {
                Texture* tex = colorAttachments[i];
                delete tex;
                tex = 0;
            }
        }

        colorAttachments.clear();

        if (rbo != 0) { // TODO renomera para rbo
            glDeleteRenderbuffers(1, &rbo);
            rbo = 0;
        }

        rbo = 0;
    }
}

void FrameBuffer::invalidade() {
    this->destroy();

    glGenFramebuffers(1, &framBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);

    // Attachment color
    if (colorTexSpecs.size() > 0) {
        colorAttachments.reserve(colorTexSpecs.size());

        int index = 0;
        for (const TexParam& textureParam : colorTexSpecs) {
            // const TexParam& textureParam = cas.textureParameters;

            Texture* texture = new Texture("", spec.width, spec.height, textureParam);
            colorAttachments.emplace_back(texture);

            const uint32_t tId = texture->getTextureID();

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, tId, 0);

            index++;
        }
    }

    if (colorAttachments.size() > 1) {
        // TODO: verificar se < de 4
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(colorAttachments.size(), buffers);
    } else if (colorAttachments.empty()) {
        // so depth-pass
        glDrawBuffer(GL_NONE);
    }

    // depth Texture
    if (!Aux::textureParameterIsUndefined(depthTexSpec)) {

        depthAttachment = new Texture("", spec.width, spec.height, depthTexSpec);

        GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment->getTextureID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // depth R.B.O.
    if (!Aux::textureParameterIsUndefined(rboSpec)) {

        TexFormat tf = rboSpec.format;          // GL_DEPTH_COMPONENT
        TexFormat tfi = rboSpec.internalFormat; // GL_DEPTH_ATTACHMENT

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)tf, spec.width, spec.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)tfi, GL_RENDERBUFFER, rbo);
    }

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw Exception(std::string("Falha em instanciar o Frame Buffer"));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const {
    glViewport(0, 0, spec.width, spec.height);
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);

    GLbitfield mask = 0;
    if (!Aux::textureParameterIsUndefined(rboSpec) || !Aux::textureParameterIsUndefined(depthTexSpec))
        mask |= GL_DEPTH_BUFFER_BIT;

    if (colorAttachments.size() > 1)
        mask |= GL_COLOR_BUFFER_BIT;

    glClear(mask);
}

void FrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::resize(const uint32_t& width, const uint32_t& height) {

    if (width == 0 || height == 0 || width > maxFrameBufferSize || height > maxFrameBufferSize) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Framebuffer resize erros Erro");
        return;
    }

    spec.width = width;
    spec.height = height;
    this->invalidade();
}

int FrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y) {

    int pixelData = 0;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

    return pixelData;
}

void FrameBuffer::clearAttachment(uint32_t attachmentIndex, const int value) {

    const TexParam& tp = colorTexSpecs[attachmentIndex];
    const TexFormat& tf = tp.format;

    glClearTexImage(colorAttachments[attachmentIndex]->getTextureID(), 0, (GLenum)tf, GL_INT, &value);
}
} // namespace Chimera