#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

static uint getBufferType(BufferType type) { // SPBufferUsageToOpenGL
    switch (type) {
        case BufferType::STATIC:
            return GL_STATIC_DRAW;
        case BufferType::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        case BufferType::STREAM:
            return GL_STREAM_DRAW;
    }
    return 0;
}

VertexBuffer::VertexBuffer(BufferType type) : type(type) { glGenBuffers(1, &bufferID); }

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &bufferID); }

void VertexBuffer::reSize(const uint32_t& size) { glBufferData(GL_ARRAY_BUFFER, size * layout.getStride(), nullptr, getBufferType(type)); }

void VertexBuffer::setLayout(const BufferLayout& bufferLayout) { // bind antes necessario!!! enableVertexAttribArray

    layout = bufferLayout;
    const std::vector<BufferElement>& elements = bufferLayout.getLayout();
    for (uint8_t i = 0; i < elements.size(); i++) {

        const BufferElement& element = elements[i];
        glEnableVertexAttribArray(i);
        if (elements.size() > 1)
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), BUFFER_OFFSET(element.offset));
        else
            glVertexAttribPointer(i, element.count, element.type, element.normalized, 0, BUFFER_OFFSET(0));
    }
    // for (uint8_t i = 0; i < elements.size(); i++) // TODO: verificar se precisa mesmo
    //     glDisableVertexAttribArray(i);
}

void VertexBuffer::setData(const void* data, const uint32_t& size) {
    glBufferData(GL_ARRAY_BUFFER, size * layout.getStride(), data, getBufferType(type));
}

// GetPointerInternal //bind anter necessario
void* VertexBuffer::map() { return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); }
// ReleasePointer
void VertexBuffer::unmap() { glUnmapBuffer(GL_ARRAY_BUFFER); }

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

} // namespace Chimera