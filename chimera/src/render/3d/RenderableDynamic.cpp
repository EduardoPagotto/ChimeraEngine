#include "chimera/render/3d/RenderableDynamic.hpp"
#include "chimera/render/OpenGLDefs.hpp"

namespace Chimera {

RenderableDynamic::RenderableDynamic(const uint32_t& max) : max(max) {
    vbo = new VertexBuffer(BufferType::STREAM); //????
    vbo->bind();

    BufferLayout layout;
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(2, GL_FLOAT, sizeof(float), false);
    vbo->setLayout(layout);
    vbo->reSize(max);
    // vbo->setData(vertexData, vertexSize);
    // vbo->releaseAtributes();
    vbo->unbind();
}

RenderableDynamic::~RenderableDynamic() {
    delete vbo;
    vbo = nullptr;
}

void RenderableDynamic::render(VertexData* pVertice, const uint32_t& size) {
    vao.bind();
    vbo->bind();
    // //int tot = size * sizeof(VertexData);
    // // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    // vbo->reSize(size);
    // vbo->setData() ??? criar funcao com glBufferSubData!!!
    // glBufferSubData(GL_ARRAY_BUFFER, 0, tot, pVertice);
    // glDrawArrays(GL_TRIANGLES, 0, size);
    vbo->unbind();
    vao.unbind();
}
} // namespace Chimera