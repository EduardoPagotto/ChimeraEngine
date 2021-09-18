#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/VertexData.hpp"

namespace Chimera ::Core {

IndexBuffer::IndexBuffer(uint32_t* data, const uint32_t& size) : size(size) {
    // Create IndexBuffer
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &bufferID); }
} // namespace Chimera::Core