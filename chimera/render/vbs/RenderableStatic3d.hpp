#ifndef __CHIMERA_RENDERABLESTATIC3D__HPP
#define __CHIMERA_RENDERABLESTATIC3D__HPP

#include "IRenderable.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include <deque>

namespace Chimera {

class RenderableStatic3d : public IRenderable {
  public:
    RenderableStatic3d(VertexData* pVertexList, const unsigned int& vertSize, uint32_t* pIndexList, const unsigned int& indexSize,
                       Shader* pShader);
    virtual ~RenderableStatic3d();

    void debugDados();
    uint32_t getSize() const { return size; }
    Shader* getShader() const { return pShader; }
    Core::VertexArray* getVao() const { return vao; }
    Core::IndexBuffer* getIBO() const { return ibo; }
    AABB* getAABB() { return &aabb; }

  private:
    AABB aabb;
    unsigned int size;
    Core::IndexBuffer* ibo;
    Core::VertexBuffer* vbo;
    Core::VertexArray* vao;
    Shader* pShader;
};

class SimpleRender3d : public IRender3d {
  public:
    virtual void submit(IRenderable* renderable) override;
    virtual void flush() override;

  private:
    std::deque<IRenderable*> renderQueue;
};

} // namespace Chimera
#endif