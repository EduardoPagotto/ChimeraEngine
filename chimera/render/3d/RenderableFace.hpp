#ifndef __CHIMERA_RENDABLE_FACE__HPP
#define __CHIMERA_RENDABLE_FACE__HPP

#include "IRenderable3d.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <vector>

namespace Chimera {

class RenderableFace : public IRenderable3d {
  public:
    RenderableFace();
    virtual ~RenderableFace();

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return indexTris.size(); }
    virtual Shader* getShader() const override { return nullptr; };
    virtual VertexArray* getVao() const override { return nullptr; };
    virtual IndexBuffer* getIBO() const override { return ibo; };
    virtual AABB* getAABB() override { return &aabb; }
    virtual void submit(Camera* camera, IRenderer3d* renderer) override;
    virtual Entity getEntity() const override { return entity; }

    inline bool empty() { return indexTris.empty(); }
    void addFace(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc);
    void initIndexBufferObject();
    void initAABB(VertexData* vertexData, const uint32_t& vertexSize);

  private:
    Entity entity;
    AABB aabb;
    IndexBuffer* ibo;
    std::vector<uint32_t> indexTris;
};
} // namespace Chimera

#endif
