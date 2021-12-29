#pragma once
#include "IRenderable3d.hpp"
#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"

namespace Chimera {

class Renderable3D : public IRenderable3d {
  public:
    Renderable3D();
    virtual ~Renderable3D();

    virtual void debugDados() const override;
    virtual uint32_t getSize() const override { return vIndex.size(); }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return ibo; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void draw(const bool& logData) override;
    inline bool empty() const { return vIndex.empty(); }

    // inicializa criando vao e ibo
    void createBuffers(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize);

    // inicializa face a face e no final cria ibo apenas (VAO e nulo)
    void addTris(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc);
    void initializeBuffer(VertexData* vertexData, const uint32_t& vertexSize);

  private:
    VertexArray* vao;
    IndexBuffer* ibo;
    AABB aabb;
    std::vector<uint32_t> vIndex;
};
} // namespace Chimera
