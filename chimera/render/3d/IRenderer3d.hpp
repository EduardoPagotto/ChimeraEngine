#ifndef __CHIMERA_IRENDER3D__ZZ1_HPP
#define __CHIMERA_IRENDER3D__ZZ1_HPP

namespace Chimera {
class IRenderer3d {
  public:
    virtual void begin(class Camera* camera) = 0;
    virtual void submit(class IRenderable3d* renderable) = 0;
    virtual void end() = 0;
    virtual void flush(bool useMaterial, class Shader* alternativeShader) = 0;
    virtual void logToggle() = 0;
    virtual bool getLog() const = 0;
};

} // namespace Chimera
#endif