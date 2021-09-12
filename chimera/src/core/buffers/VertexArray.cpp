#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera::Core {

VertexArray::VertexArray() { glGenVertexArrays(1, &arrayID); }

VertexArray::~VertexArray() {

    for (int i = 0; i < buffers.size(); i++)
        delete buffers[i];

    glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::addBuffer(VertexBuffer* buffer, uint32_t index) {

    this->bind();
    buffer->bind();

    // ----
    // glEnableVertexAttribArray(index);
    // glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
    // ---- equivale a
    // buffer->setSlot(index, buffer->getComponentCount(), BUFFER_OFFSET(0));  // Vertice
    // ----

    buffer->setSlot(0, 3, BUFFER_OFFSET(0));  // Vertice
    buffer->setSlot(1, 3, BUFFER_OFFSET(12)); // Normal
    buffer->setSlot(2, 2, BUFFER_OFFSET(24)); // Texture

    buffer->unbind();
    this->unbind();

    buffer->unsetSlot(0);
    buffer->unsetSlot(1);
    buffer->unsetSlot(2);
}
} // namespace Chimera::Core