#include "chimera/core/Vertex.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

#define BUFFER_OFFSET(i) ((void*)(i))

VertexFull::VertexFull() {}

void VertexFull::render(std::vector<VertexDataFull>& vVertice) {

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int tot = vVertice.size() * sizeof(Chimera::VertexDataFull);
    // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tot, &vVertice[0]);

    glDrawArrays(GL_TRIANGLES, 0, vVertice.size()); //?? https://www.youtube.com/watch?v=S_xUgzFMIso

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexFull::create(const unsigned int& max) {

    // refs: ver Particle.cpp in node sub-projetc
    // http://www.songho.ca/opengl/gl_vbo.html#create
    // https://www.khronos.org/opengl/wiki/Vertex_Rendering

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, max, nullptr, GL_STREAM_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(1);

    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(24));
    glEnableVertexAttribArray(2);

    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(36));
    glEnableVertexAttribArray(3);

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

VertexFull::~VertexFull() {
    // FIXME: remover da memoria o VAO/VBO
}

} // namespace Chimera