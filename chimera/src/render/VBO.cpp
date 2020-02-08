#include "chimera/render/VBO.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

VBO::VBO() : vbo(0) {}
VBO::~VBO() {
    if (vbo > 0)
        glDeleteBuffers(1, &vbo);
}

void VBO::setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset) {
    glVertexAttribPointer(slotID, slotSize, GL_FLOAT, GL_FALSE, sizeof(VertexData), offset);
    glEnableVertexAttribArray(slotID);
}

void VBO::initialize(std::vector<VertexData>& vertexData, const int& maxBufferSize) {

    // cria o vao
    vao.create();
    vao.bind();

    // Buffer de vertice
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (maxBufferSize == 0) {

        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), &vertexData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // TODO: implementar em herança
        createIndex();

        // vincula vbo
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    } else {

        glBufferData(GL_ARRAY_BUFFER, maxBufferSize, nullptr, GL_STREAM_DRAW);
    }

    this->setSlot(0, 3, BUFFER_OFFSET(0));  // Vertice
    this->setSlot(1, 3, BUFFER_OFFSET(12)); // Normal
    this->setSlot(2, 2, BUFFER_OFFSET(24)); // Texture

    // vincula ibo
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    if (maxBufferSize == 0) {
        clearIndex();
    }

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vao.unbind();

    glDisableVertexAttribArray(0); // Slot 0 Vertice
    glDisableVertexAttribArray(1); // Slot 1 Normal
    glDisableVertexAttribArray(2); // Slot 2 Textura
}

} // namespace Chimera