#include "chimera/render/vbs/RenderableDynamic.hpp"

namespace Chimera {

RenderableDynamic::RenderableDynamic(const uint32_t& max) : max(max) {

    vbo = new Core::VertexBuffer(Core::BufferType::STREAM); //????
    vbo->bind();
    vbo->setLayout(vertexDataComponentes());
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