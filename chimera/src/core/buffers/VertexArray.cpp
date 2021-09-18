#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera::Core {

VertexArray::VertexArray() { glGenVertexArrays(1, &arrayID); }

VertexArray::~VertexArray() {

    for (int i = 0; i < buffers.size(); i++)
        delete buffers[i];

    glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::addBuffer(VertexBuffer* buffer) {

    this->bind();
    buffer->bind();

    buffer->enableVertexAttribArray();
    buffer->unbind();
    this->unbind();
    buffer->disableVertexAttribArray();
}
} // namespace Chimera::Core