#include "chimera/render/vbs/VertexDynamic.hpp"

namespace Chimera {

VertexRenderDynamic::VertexRenderDynamic(const unsigned int& max) { this->vbo = new Core::VertexBuffer(nullptr, max, 3); }

VertexRenderDynamic::~VertexRenderDynamic() {
    delete vbo;
    vbo = nullptr;
}

void VertexRenderDynamic::render(VertexData* pVertice, const unsigned int size) {

    vao.bind();
    vbo->bind();

    int tot = size * sizeof(VertexData);
    // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tot, pVertice);
    glDrawArrays(GL_TRIANGLES, 0, size);

    vbo->unbind();
    vao.unbind();
}
} // namespace Chimera