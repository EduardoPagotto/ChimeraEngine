#include "chimera/render/VertexDynamic.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

VertexRenderDynamic::VertexRenderDynamic() {}

VertexRenderDynamic::~VertexRenderDynamic() {}

void VertexRenderDynamic::render(std::vector<VertexData>& vVertice) {

    vao.bind();

    vbo.bind(); // glBindBuffer(GL_ARRAY_BUFFER, vboGL);

    int tot = vVertice.size() * sizeof(Chimera::VertexData);
    // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tot, &vVertice[0]);

    glDrawArrays(GL_TRIANGLES, 0, vVertice.size());

    vbo.unbind(); // glBindBuffer(GL_ARRAY_BUFFER, 0);

    vao.unbind();
}

void VertexRenderDynamic::create(const unsigned int& max) {

    vao.create();
    vao.bind();
    vbo.buildDynamic(max);
}
} // namespace Chimera