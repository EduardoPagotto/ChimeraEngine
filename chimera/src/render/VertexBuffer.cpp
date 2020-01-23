#include "chimera/render/VertexBuffer.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

VertexBuffer::VertexBuffer(bool _bufferDynamic) : vbo(0), vao(0), bufferDynamic(_bufferDynamic) {}
VertexBuffer::~VertexBuffer() {
    if (vao > 0)
        glDeleteVertexArrays(1, &vao);

    if (vbo > 0)
        glDeleteBuffers(1, &vbo);
}

void VertexBuffer::initialize(std::vector<VertexData>& vertexData) {

    // cria o vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Buffer de vertice
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (bufferDynamic == false) {

        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), &vertexData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // TODO: implementar em herança
        createIndex();

        // vincula vbo
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    } else {

        glBufferData(GL_ARRAY_BUFFER, max, nullptr, GL_STREAM_DRAW);
    }

    // Vertice
    GLuint positionID = 0; // glGetAttribLocation(shader.getIdProgram(), "position");
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(positionID);

    // Normal
    GLuint normalID = 1; // glGetAttribLocation(shader.getIdProgram(), "normal");
    glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(normalID);

    // Texture
    GLuint uvID = 2; // glGetAttribLocation(shader.getIdProgram(), "uv1");
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(24));
    glEnableVertexAttribArray(uvID);

    // vincula ibo
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    if (bufferDynamic == false) {
        clearIndex();
    }

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(positionID);
    glDisableVertexAttribArray(normalID);
    glDisableVertexAttribArray(uvID);
}

} // namespace Chimera