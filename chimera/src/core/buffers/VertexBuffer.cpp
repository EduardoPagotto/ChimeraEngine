#include "chimera/core/buffers/VertexBuffer.hpp"

namespace Chimera::Core {

// https://www.youtube.com/watch?v=qTGMXcFLk2E&t=2063s

VertexBuffer::VertexBuffer(VertexData* vertexData, const uint32_t& size, VertexComponents* components) : components(components) {

    this->size = size;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    if (vertexData != nullptr) {
        glBufferData(GL_ARRAY_BUFFER, size * components->sizeVertex, vertexData, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size * components->sizeVertex, nullptr, GL_STREAM_DRAW);
    }
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &bufferID);
    delete components;
}

void VertexBuffer::enableVertexAttribArray() {

    for (auto slot : components->loc) {
        glVertexAttribPointer(slot.id, slot.size, slot.type, slot.normalize, components->sizeVertex, slot.offset);
        glEnableVertexAttribArray(slot.id);
    }
}

void VertexBuffer::disableVertexAttribArray() {

    for (auto slot : components->loc) {
        glDisableVertexAttribArray(slot.id);
    }
}

} // namespace Chimera::Core