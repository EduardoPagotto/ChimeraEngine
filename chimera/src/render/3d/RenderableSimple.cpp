#include "chimera/render/3d/RenderableSimple.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableSimple::RenderableSimple(Entity entity) {

    this->entity = entity;
    MeshData& mesh = entity.getComponent<MeshData>();

    std::vector<Chimera::VertexData> renderData;
    vertexDataFromMesh(&mesh, renderData);

    std::vector<uint32_t> index;
    std::vector<Chimera::VertexData> vertexDataOut;
    vertexDataIndexCompile(renderData, vertexDataOut, index);

    createBuffers(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());
}

void RenderableSimple::createBuffers(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize) {

    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(2, GL_FLOAT, sizeof(float), false);

    vbo->setLayout(layout);
    vbo->setData(vertexData, vertexSize);
    vbo->unbind();
    vao->push(vbo);

    vao->unbind();

    ibo = new IndexBuffer(indexData, indexSize);

    // init AABB
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, indexData, indexSize, min, max, size);
    aabb.setBoundary(min, max);
}

RenderableSimple::~RenderableSimple() {
    delete vao;
    delete ibo;
}

void RenderableSimple::submit(Camera* camera, Renderer3d* renderer) { renderer->submit(this); }

void RenderableSimple::debugDados() {
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
    aabb.render();
}
} // namespace Chimera