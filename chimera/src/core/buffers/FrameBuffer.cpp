#include "chimera/core/buffers/FrameBuffer.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"

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

FrameBufferZ::FrameBufferZ(const FrameBufferSpecification& spec) : spec(spec), framBufferID(0), rbo(0) {

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

FrameBufferZ::~FrameBufferZ() { this->destroy(); }

void FrameBufferZ::destroy() {

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

void FrameBufferZ::invalidade() {
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

void FrameBufferZ::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);
    glViewport(0, 0, spec.width, spec.height);

    if (!Aux::textureParameterIsUndefined(depthTexSpec)) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}

void FrameBufferZ::clearDepth(const glm::vec4& value) const {
    glClearColor(value.x, value.y, value.z, value.w);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void FrameBufferZ::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBufferZ::resize(const uint32_t& width, const uint32_t& height) {

    if (width == 0 || height == 0 || width > maxFrameBufferSize || height > maxFrameBufferSize) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Framebuffer resize erros Erro");
        return;
    }

    spec.width = width;
    spec.height = height;
    this->invalidade();
}

int FrameBufferZ::readPixel(uint32_t attachmentIndex, int x, int y) {

    int pixelData = 0;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

    return pixelData;
}

void FrameBufferZ::clearAttachment(uint32_t attachmentIndex, const int value) {

    const TexParam& tp = colorTexSpecs[attachmentIndex];
    const TexFormat& tf = tp.format;

    glClearTexImage(colorAttachments[attachmentIndex]->getTextureID(), 0, (GLenum)tf, GL_INT, &value);
}

//------------------

FrameBuffer::FrameBuffer(const uint16_t& width, const uint16_t& height) : width(width), height(height) {

    // pass 1 cria framebuffer // FramebufferName => fbo
    glGenFramebuffers(1, &framBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framBufferID);

    // pass 2 create textura that will be used as a color buffer // renderedTexture => texture
    TexParam params(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE);
    texture = new Texture("", width, height, params);

    // Pass 4 //Set "renderedTexture" as our colour attachement #0
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->getTextureID(), 0);

    // Set the list of draw buffers. // ??????
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // pass 3  // The depth buffer // depthrenderbuffer => fb_depth
    glGenRenderbuffers(1, &depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

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