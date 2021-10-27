#include "fb.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

static const uint32_t maxFrameBufferSize = 8192;

namespace Utils {

static bool isDepthFormat(FramebufferTextureFormat format) {
    switch (format) {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            break;
    }

    return false;
}

static uint32_t TextureTarget(bool multisampled) { return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }

static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
    glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void bindTexture(bool multisampled, uint32_t id) { glBindTexture(TextureTarget(multisampled), id); }

static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index) {

    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachementType, uint32_t width, uint32_t height) {

    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    } else {
        // glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachementType, TextureTarget(multisampled), id, 0);
}

} // namespace Utils

Framebuffer::Framebuffer(FrameBufferSpecification spec) {

    for (auto format : spec.attachments.attachments) {
        if (!Utils::isDepthFormat(format.textureFormat))
            colorAttachmentSpecifications.emplace_back(format);
        else
            depthAttachmentSpecification = format;
    }

    this->invalidade();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &renderID);

    if (!colorAttachments.empty())
        glDeleteTextures(colorAttachments.size(), colorAttachments.data());

    if (depthAttachment != 0)
        glDeleteTextures(1, &depthAttachment);
}

void Framebuffer::invalidade() {

    if (renderID) {
        glDeleteFramebuffers(1, &renderID);

        if (!colorAttachments.empty()) {
            glDeleteTextures(colorAttachments.size(), colorAttachments.data());
        }

        if (depthAttachment != 0)
            glDeleteTextures(1, &depthAttachment);

        colorAttachments.clear();
        depthAttachment = 0;
    }

    glCreateFramebuffers(1, &renderID); // glGenFramebuffers(1, &renderID);
    glBindFramebuffer(GL_FRAMEBUFFER, renderID);

    // Attachments
    bool multisample = spec.samples > 1;

    if (colorAttachmentSpecifications.size() > 0) {
        colorAttachments.resize(colorAttachmentSpecifications.size());

        Utils::CreateTextures(multisample, colorAttachments.data(), colorAttachments.size());

        for (size_t i = 0; i < colorAttachments.size(); i++) {
            Utils::bindTexture(multisample, colorAttachments[i]);

            switch (colorAttachmentSpecifications[i].textureFormat) {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(colorAttachments[i], spec.samples, GL_RGBA8, spec.width, spec.heigh, i);
                    break;

                default:
                    break;
            }
        }
    }

    if (depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None) {

        Utils::CreateTextures(multisample, &depthAttachment, 1);
        Utils::bindTexture(multisample, depthAttachment);

        switch (depthAttachmentSpecification.textureFormat) {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(depthAttachment, spec.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, spec.width,
                                          spec.heigh);
                break;

            default:
                break;
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

    // // Color Attachment
    // glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
    // glBindTexture(GL_TEXTURE_2D, colorAttachment);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.heigh, 0, GL_RGBA8, GL_UNSIGNED_BYTE, nullptr);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

    // // Depth Attachment
    // glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
    // glBindTexture(GL_TEXTURE_2D, depthAttachment);
    // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.width, spec.heigh);
    // // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec.width, spec.heigh, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8,
    // //              nullptr); // 24 to depth 8 to stencil

    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

    // check if is correct
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Framebuffer Erro");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, renderID);
    glViewport(0, 0, spec.width, spec.heigh);
}

void Framebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::resize(const uint32_t& width, const uint32_t& heigh) {

    if (width == 0 || heigh == 0 || width > maxFrameBufferSize || heigh > maxFrameBufferSize) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Framebuffer resize erros Erro");
        return;
    }

    spec.width = width;
    spec.heigh = heigh;
    this->invalidade();
}