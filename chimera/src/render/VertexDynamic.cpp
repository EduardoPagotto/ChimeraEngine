#include "chimera/render/VertexDynamic.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

VertexRenderDynamic::VertexRenderDynamic() : VBO() {}

VertexRenderDynamic::~VertexRenderDynamic() {}

void VertexRenderDynamic::render(std::vector<VertexData>& vVertice) {

    vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int tot = vVertice.size() * sizeof(Chimera::VertexData);
    // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tot, &vVertice[0]);

    glDrawArrays(GL_TRIANGLES, 0, vVertice.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vao.unbind();
}

void VertexRenderDynamic::create(const unsigned int& max) {

    std::vector<VertexData> vVertice;
    VBO::initialize(vVertice, max);
}
} // namespace Chimera