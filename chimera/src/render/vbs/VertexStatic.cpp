#include "chimera/render/vbs/VertexStatic.hpp"

namespace Chimera {

VertexRenderStatic::VertexRenderStatic(std::vector<VertexData>& vertexDataIn, std::vector<uint32_t> index) {

    std::vector<VertexData> vertexData;
    if (index.size() == 0) {
        vertexDataIndexCompile(vertexDataIn, vertexData, indexTris);
    } else {
        vertexData = vertexDataIn;
        indexTris = index;
    }

    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(&vertexData[0], vertexData.size(), 3), 0); // FIXME 0 para compatibilidade

    vao->bind();
    ibo = new Core::IndexBuffer(&indexTris[0], indexTris.size());
    vao->unbind();

    // // init AABB
    // glm::vec3 min, max, size;
    // vertexDataIndexMinMaxSize(pVertexList, vertSize, pIndexList, indexSize, min, max, size);
    // aabb.setBoundary(min, max);
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