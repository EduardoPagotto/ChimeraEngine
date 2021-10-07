#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/vbs/RenderableIBO.hpp"

namespace Chimera {

class RenderableIndex : public IRenderable {
  public:
    RenderableIndex(std::vector<RenderableIBO*>& vpLeafData, std::vector<VertexData>& vertexData);
    ~RenderableIndex();

    virtual void submit(IRenderer3d* renderer) override;
    virtual void debugDados() override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual Shader* getShader() const { return nullptr; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return nullptr; }
    virtual AABB* getAABB() { return &aabb; }

  private:
    VertexArray* vao;
    std::vector<RenderableIBO*> vpLeaf;
    std::vector<VertexData> vVertex;
    AABB aabb;
    uint32_t totIndex;
};

} // namespace Chimera
#endif
