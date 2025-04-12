#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include <stdint.h>

namespace ce {
class Renderable3D : public IRenderable3d {
  public:
    Renderable3D() = default;
    virtual ~Renderable3D();
    virtual void draw(const bool& logData);
    virtual const uint32_t getSize() const = 0;
    virtual const class AABB& getAABB() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;

    inline VertexArray* getVao() const { return vao; }
    inline void setIndexAuxCommand(const uint32_t& command) { indexAuxCommand = command; }
    inline const uint32_t getIndexAuxCommand() const { return indexAuxCommand; }

  protected:
    uint32_t indexAuxCommand = 0;
    VertexArray* vao = nullptr;
};
} // namespace ce
