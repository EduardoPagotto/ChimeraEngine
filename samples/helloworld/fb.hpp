#pragma once

#include <initializer_list>
#include <stdint.h>
#include <vector>

enum class FramebufferTextureFormat {
    None = 0,
    RGBA8,
    RED_INTEGER,
    DEPTH24STENCIL8,
    // default
    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format) : textureFormat(format) {}
    FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
    // TODO: filter/wrap
};

struct FramebufferAttachmentSpecification {
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> _attachments) : attachments(_attachments) {}
    std::vector<FramebufferTextureSpecification> attachments;
};

struct FrameBufferSpecification {
    uint32_t width = 800, heigh = 600;
    FramebufferAttachmentSpecification attachments;
    bool swapChainTarget = false;
    int samples = 1;
};

class Framebuffer {
  public:
    Framebuffer(FrameBufferSpecification spec);
    ~Framebuffer();
    void invalidade();

    // usado como o textureid para incular ao um drawcall
    uint32_t getColorAttachemntRenderID(uint32_t index = 0) const { return colorAttachments[index]; }

    int readPixel(uint32_t attachmentIndex, int x, int y);
    void clearAttachment(uint32_t attachmentIndex, const int value);

    void bind() const;
    void unbind() const;
    void resize(const uint32_t& width, const uint32_t& heigh);

  private:
    uint32_t renderID = 0;
    FrameBufferSpecification spec;
    std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
    FramebufferTextureSpecification depthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<uint32_t> colorAttachments;
    uint32_t depthAttachment = 0;
};
