#include "chimera/render/vbs/VBO.hpp"

namespace Chimera {

// https://www.youtube.com/watch?v=qTGMXcFLk2E&t=2063s

VBO::VBO(std::vector<VertexData>* vertexData, const unsigned int& componentCount) : bufferID(0) {

    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    if (vertexData != nullptr) {
        // glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), &vertexData[0], GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, componentCount, &(*vertexData)[0], GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, componentCount, nullptr, GL_STREAM_DRAW);
    }

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

VBO::~VBO() {
    if (bufferID > 0)
        glDeleteBuffers(1, &bufferID);
}

void VBO::setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset) {
    glVertexAttribPointer(slotID, slotSize, GL_FLOAT, GL_FALSE, sizeof(VertexData), offset);
    glEnableVertexAttribArray(slotID);
}
} // namespace Chimera