#include "chimera/core/buffers/Buffer.hpp"

namespace Chimera::Core {

// https://www.youtube.com/watch?v=qTGMXcFLk2E&t=2063s

Buffer::Buffer(VertexData* vertexData, const unsigned int& size, const unsigned int& componentCount) : componentCount(componentCount) {

    this->size = size;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    if (vertexData != nullptr) {
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), nullptr, GL_STREAM_DRAW);
    }
}

Buffer::~Buffer() { glDeleteBuffers(1, &bufferID); }

void Buffer::setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset) {
    glVertexAttribPointer(slotID, slotSize, GL_FLOAT, GL_FALSE, sizeof(VertexData), offset);
    glEnableVertexAttribArray(slotID);
}
} // namespace Chimera::Core