#ifndef __CHIMERA_FRAME_BUFFER__HPP
#define __CHIMERA_FRAME_BUFFER__HPP

#include "chimera/core/Texture.hpp"
#include <glm/glm.hpp>

namespace Chimera {

struct FrameBufferSpecification {
    FrameBufferSpecification() = default;
    uint32_t width = 800, height = 600;
    std::vector<TexParam> attachments;
    bool swapChainTarget = false;
    int samples = 1;
};

class FrameBuffer {
  public:
    FrameBuffer(const FrameBufferSpecification& spec);
    ~FrameBuffer();

    void bind() const;
    static void unbind();
    void clearAttachment(uint32_t attachmentIndex, const int value);
    void resize(const uint32_t& width, const uint32_t& height);
    int readPixel(uint32_t attachmentIndex, int x, int y);
    inline Texture* getColorAttachemnt(uint32_t index) const { return colorAttachments[index]; } //
    inline const uint32_t getWidth() const { return spec.width; }
    inline const uint32_t getHeight() const { return spec.height; }
    inline Texture* getDepthAttachemnt() { return depthAttachment; }
    // void clearDepth(const glm::vec4& value) const;

  private:
    void destroy();
    void invalidade();

    uint32_t framBufferID, rbo;
    FrameBufferSpecification spec;
    Texture* depthAttachment;
    TexParam depthTexSpec, rboSpec;
    std::vector<Texture*> colorAttachments;
    std::vector<TexParam> colorTexSpecs;
};

} // namespace Chimera
#endif