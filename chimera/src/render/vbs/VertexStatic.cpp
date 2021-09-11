#include "chimera/render/vbs/VertexStatic.hpp"

namespace Chimera {

VertexRenderStatic::VertexRenderStatic(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index) {

    std::vector<VertexData> vertexData;
    if (index.size() == 0) {
        vertexDataIndexCompile(vertexDataIn, vertexData, indexIBO);
    } else {
        vertexData = vertexDataIn;
        indexIBO = index;
    }

    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(&vertexData[0], vertexData.size(), 3), 0); // FIXME 0 para compatibilidade

    vao->bind();
    ibo = new Core::IndexBuffer(&indexIBO[0], indexIBO.size());
    vao->unbind();
}

VertexRenderStatic::~VertexRenderStatic() { delete vao; }

void VertexRenderStatic::render() {
    vao->bind();
    ibo->render();
    vao->unbind();
}
} // namespace Chimera