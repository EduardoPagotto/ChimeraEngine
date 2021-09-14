#ifndef __CHIMERA_VERTEX_NODE__HPP
#define __CHIMERA_VERTEX_NODE__HPP

#include "IRenderable.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/render/AABB.hpp"
#include <vector>

namespace Chimera {

class RenderableIBO : public IRenderable {
  public:
    RenderableIBO();
    ~RenderableIBO();

    inline bool empty() { return indexTris.empty(); }
    void addFace(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc);
    void initIndexBufferObject();
    void initAABB(VertexData* vertexData, const uint32_t& vertexSize);

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return indexTris.size(); }
    virtual Shader* getShader() const { return nullptr; };
    virtual Core::VertexArray* getVao() const { return nullptr; };
    virtual Core::IndexBuffer* getIBO() const { return ibo; };
    virtual AABB* getAABB() override { return &aabb; }
    virtual void inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue) override;

  private:
    AABB aabb;
    Core::IndexBuffer* ibo;
    std::vector<uint32_t> indexTris;
};
} // namespace Chimera

#endif
