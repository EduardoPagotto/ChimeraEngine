#include "chimera/core/buffers/VertexBuffer.hpp"

namespace Chimera::Core {

// https://www.youtube.com/watch?v=qTGMXcFLk2E&t=2063s

VertexBuffer::VertexBuffer(VertexData* vertexData, const unsigned int& size, const unsigned int& componentCount)
    : componentCount(componentCount) {

    this->size = size;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    if (vertexData != nullptr) {
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), nullptr, GL_STREAM_DRAW);
    }
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &bufferID); }

void VertexBuffer::setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset) {
    glVertexAttribPointer(slotID, slotSize, GL_FLOAT, GL_FALSE, sizeof(VertexData), offset);
    glEnableVertexAttribArray(slotID);
}
} // namespace Chimera::Core