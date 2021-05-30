#include "chimera/render/vbs/VBO.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

VBO::VBO() : vboGL(0) {}
VBO::~VBO() {
    if (vboGL > 0)
        glDeleteBuffers(1, &vboGL);
}

void VBO::setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset) {
    glVertexAttribPointer(slotID, slotSize, GL_FLOAT, GL_FALSE, sizeof(VertexData), offset);
    glEnableVertexAttribArray(slotID);
}

void VBO::bind() { glBindBuffer(GL_ARRAY_BUFFER, vboGL); }
void VBO::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::building() {

    // Vertex fields ids
    this->setSlot(0, 3, BUFFER_OFFSET(0));  // Vertice
    this->setSlot(1, 3, BUFFER_OFFSET(12)); // Normal
    this->setSlot(2, 2, BUFFER_OFFSET(24)); // Texture

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0); // Slot 0 Vertice
    glDisableVertexAttribArray(1); // Slot 1 Normal
    glDisableVertexAttribArray(2); // Slot 2 Textura
}

void VBO::buildStatic(std::vector<VertexData>& vertexData) {
    // Buffer de vertice
    glGenBuffers(1, &vboGL);
    glBindBuffer(GL_ARRAY_BUFFER, vboGL);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), &vertexData[0], GL_STATIC_DRAW);

    this->building();
}

void VBO::buildDynamic(const int& maxBufferSize) {
    // Buffer de vertice
    glGenBuffers(1, &vboGL);
    glBindBuffer(GL_ARRAY_BUFFER, vboGL);
    glBufferData(GL_ARRAY_BUFFER, maxBufferSize, nullptr, GL_STREAM_DRAW);

    this->building();
}
} // namespace Chimera