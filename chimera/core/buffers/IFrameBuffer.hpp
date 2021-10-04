#ifndef __CHIMERA_IFRAME_BUFFER__HPP
#define __CHIMERA_IFRAME_BUFFER__HPP

namespace Chimera {

class IFrameBuffer {
  public:
    virtual class Texture* getTexture() const = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void clear() = 0;
};
} // namespace Chimera
#endif