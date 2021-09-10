#include "chimera/render/vbs/VertexStatic.hpp"

namespace Chimera {

VertexRenderStatic::VertexRenderStatic(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index) {

    if (index.size() == 0) {
        vertexDataIndexCompile(vertexDataIn, vertexData, indexIBO);
    } else {
        vertexData = vertexDataIn;
        indexIBO = index;
    }

    vao.create();
    vao.bind();
    vbo = new VBO(&vertexData, vertexData.size() * sizeof(VertexData));

    vao.bind();
    ebo.create(indexIBO);
    vao.unbind();
}

VertexRenderStatic::~VertexRenderStatic() {
    delete vbo;
    vbo = nullptr;
}

void VertexRenderStatic::render() {
    vao.bind();
    ebo.render();
    vao.unbind();
}
} // namespace Chimera