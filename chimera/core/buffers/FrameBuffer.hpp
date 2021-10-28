#ifndef __CHIMERA_FRAME_BUFFER__HPP
#define __CHIMERA_FRAME_BUFFER__HPP

#include "IFrameBuffer.hpp"
#include "chimera/core/Texture.hpp"
#include <glm/glm.hpp>

namespace Chimera {

// Contem todos os parmetros com propriedades de textura
struct FramebufferTextureSpecification {
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(TextureParameters params) : textureParameters(params) {}
    TextureParameters textureParameters = TextureParameters();
};

struct FramebufferAttachmentSpecification {
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> _attachments) : attachments(_attachments) {}
    std::vector<FramebufferTextureSpecification> attachments;
};

struct FrameBufferSpecification {
    FrameBufferSpecification() = default;
    uint32_t width = 800, height = 600;
    FramebufferAttachmentSpecification attachments;
    bool swapChainTarget = false;
    int samples = 1;
};

class FrameBufferZ { // TODO: continuar daqui!!!!
  public:
    FrameBufferZ(const FrameBufferSpecification& spec);
    ~FrameBufferZ();
    void destroy();
    void invalidade();
    void bind() const;
    static void unbind();
    void clearAttachment(uint32_t attachmentIndex, const int value);
    void resize(const uint32_t& width, const uint32_t& height);
    int readPixel(uint32_t attachmentIndex, int x, int y);
    inline Texture* getColorAttachemnt(uint32_t index) const { return colorAttachments[index]; } //

  private:
    // uint16_t width, height;
    uint32_t framBufferID, depthAttachment;
    FrameBufferSpecification spec;

    std::vector<Texture*> colorAttachments;
    std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
    FramebufferTextureSpecification depthAttachmentSpecification;
};

class FrameBuffer : public IFrameBuffer {
  public:
    FrameBuffer(const uint16_t& width, const uint16_t& height);
    virtual ~FrameBuffer();

    inline Texture* getTexture() const override { return texture; }
    void bind() const override;
    void unbind() const override;
    void clear() override;
    inline void setClearColor(const glm::vec4& color) { clearColor = color; }

  private:
    uint16_t width, height;
    uint32_t framBufferID, depthBufferID;
    Texture* texture;
    glm::vec4 clearColor;
};

} // namespace Chimera
#endif