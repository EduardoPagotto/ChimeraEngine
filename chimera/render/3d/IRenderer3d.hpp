#ifndef __CHIMERA_IRENDER3D__ZZ1_HPP
#define __CHIMERA_IRENDER3D__ZZ1_HPP

namespace Chimera {
class IRenderer3d {
  public:
    virtual void begin(class ICamera* camera) = 0;
    virtual void submitLight(class Light* light) = 0;
    virtual void submit(const class RenderCommand& command) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual void logToggle() = 0;
    virtual bool getLog() const = 0;
};

} // namespace Chimera
#endif