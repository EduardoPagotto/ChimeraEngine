#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/render/vbs/RenderableIBO.hpp"

namespace Chimera {

class HeightMap : public IRenderable {
  public:
    HeightMap(std::vector<RenderableIBO*>& vpLeafData, std::vector<VertexData>& vertexData);
    ~HeightMap();

    virtual void inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue);
    virtual void debugDados() {}
    virtual uint32_t getSize() const { return totIndex; }
    virtual Shader* getShader() const { return nullptr; }
    virtual Core::VertexArray* getVao() const { return vao; }
    virtual Core::IndexBuffer* getIBO() const { return 0; }
    virtual AABB* getAABB() { return &aabb; }

  private:
    Core::VertexArray* vao;
    std::vector<RenderableIBO*> vpLeaf;
    std::vector<VertexData> vVertex;
    AABB aabb;
    uint32_t totIndex;
};

} // namespace Chimera
#endif
