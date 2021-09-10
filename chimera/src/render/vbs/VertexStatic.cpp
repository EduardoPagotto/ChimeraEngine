#include "chimera/render/vbs/VertexStatic.hpp"

namespace Chimera {

VertexRenderStatic::VertexRenderStatic(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index) {

    if (index.size() == 0) {
        vertexDataIndexCompile(vertexDataIn, vertexData, indexIBO);
    } else {
        vertexData = vertexDataIn;
        indexIBO = index;
    }

    vao = new Core::VertexArray();
    vao->addBuffer(new Core::Buffer(&vertexData, vertexData.size() * sizeof(VertexData)), 0); // FIXME 0 para compatibilidade

    vao->bind();
    ebo = new Core::IndexBuffer(indexIBO);
    vao->unbind();
}

VertexRenderStatic::~VertexRenderStatic() { delete vao; }

void VertexRenderStatic::render() {
    vao->bind();
    ebo->render();
    vao->unbind();
}
} // namespace Chimera