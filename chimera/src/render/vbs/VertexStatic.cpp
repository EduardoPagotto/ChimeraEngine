#include "chimera/render/VertexStatic.hpp"

namespace Chimera {

VertexRenderStatic::VertexRenderStatic() {}
VertexRenderStatic::~VertexRenderStatic() {

    if (vertexData.size() > 0)
        vertexData.clear();

    if (indexIBO.size() > 0)
        indexIBO.clear();
}

void VertexRenderStatic::render() {
    vao.bind();
    ebo.render();
    vao.unbind();
}

void VertexRenderStatic::create(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index) {

    if (index.size() == 0) {
        vertexDataIndexCompile(vertexDataIn, vertexData, indexIBO);
    } else {
        vertexData = vertexDataIn;
        indexIBO = index;
    }

    vao.create();
    vao.bind();
    vbo.buildStatic(vertexData);

    vao.bind();
    ebo.create(indexIBO);
    vao.unbind();
}
} // namespace Chimera