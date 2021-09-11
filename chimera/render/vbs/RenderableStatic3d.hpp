#ifndef __CHIMERA_RENDERABLESTATIC3D__HPP
#define __CHIMERA_RENDERABLESTATIC3D__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/render/AABB.hpp"
#include <deque>

namespace Chimera {

class RenderableStatic3d {
  public:
    RenderableStatic3d(VertexData* pVertexList, const unsigned int& vertSize, uint32_t* pIndexList, const unsigned int& indexSize,
                       Shader* pShader);
    virtual ~RenderableStatic3d();

    void debugDados();
    uint32_t getSize() const { return size; }
    Core::VertexArray* getVao() const { return vao; }
    Core::IndexBuffer* getIBO() const { return ibo; }
    Shader* getShader() const { return pShader; }
    const AABB& getAABB() { return aabb; }

    AABB aabb;

  private:
    unsigned int size;
    Core::IndexBuffer* ibo;
    Core::VertexBuffer* vbo;
    Core::VertexArray* vao;
    Shader* pShader;
};

class IRender3d {
  public:
    virtual void submit(RenderableStatic3d* renderable) = 0;
    virtual void flush() = 0;
};

class SimpleRender3d : IRender3d {
  public:
    virtual void submit(RenderableStatic3d* renderable) override;
    virtual void flush() override;

  private:
    std::deque<RenderableStatic3d*> renderQueue;
};

} // namespace Chimera
#endif