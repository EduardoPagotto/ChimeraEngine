#include "chimera/render/vbs/VertexStatic.hpp"

namespace Chimera {

VertexRenderStatic::VertexRenderStatic(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize) {

    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(vertexData, vertexSize, 3), 0); // FIXME 0 para compatibilidade

    vao->bind();
    ibo = new Core::IndexBuffer(indexData, indexSize);
    vao->unbind();

    // init AABB
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, indexData, indexSize, min, max, size);
    aabb.setBoundary(min, max);
}

VertexRenderStatic::~VertexRenderStatic() {
    delete vao;
    delete ibo;
}

void VertexRenderStatic::render() {
    vao->bind();
    // ibo->render();
    ibo->bind();
    glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    ibo->unbind();
    //---
    vao->unbind();
}

void VertexRenderStatic::debugDados() {} // TODO: implementar debugdados

} // namespace Chimera