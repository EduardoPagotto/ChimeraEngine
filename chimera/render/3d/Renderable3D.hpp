#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include <stdint.h>

namespace Chimera {
class Renderable3D : public IRenderable3d {
  public:
    Renderable3D() = default;
    virtual ~Renderable3D();
    virtual VertexArray* getVao() const { return vao; }
    virtual void draw(const bool& logData);
    virtual const uint32_t getSize() const = 0;
    virtual const class AABB& getAABB() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;

    void setAux(const uint32_t& command, const uint32_t& draw);
    inline const uint32_t getIndexAuxCommand() const { return indexAuxCommand; }
    inline const uint32_t getIndexAuxDraw() const { return indexAuxDraw; }

  protected:
    uint32_t indexAuxCommand = 0;
    uint32_t indexAuxDraw = 0;
    VertexArray* vao = nullptr;
};
} // namespace Chimera
