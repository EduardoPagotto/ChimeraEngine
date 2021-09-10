#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/VertexData.hpp"

namespace Chimera ::Core {

IndexBuffer::IndexBuffer(std::vector<uint32_t>& index) {

    uint32_t sizeBufferIndex = index.size() * sizeof(uint32_t);
    sizeIndex = index.size();

    // Create IndexBuffer
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &index[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &bufferID); }

void IndexBuffer::render() {
    this->bind();
    glDrawElements(GL_TRIANGLES, sizeIndex, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    this->unbind();
}
} // namespace Chimera::Core