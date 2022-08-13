#include "chimera/core/buffer/VertexBuffer.hpp"

namespace Chimera {

VertexBuffer::VertexBuffer(BufferType type) : type(type) { glGenBuffers(1, &bufferID); }

VertexBuffer::VertexBuffer(BufferType type, const uint32_t& size, void* data) {
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, (GLuint)type);
    sizeTotal = size;
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &bufferID); }

void VertexBuffer::reSize(const uint32_t& size) { glBufferData(GL_ARRAY_BUFFER, size * layout.getStride(), nullptr, (GLuint)type); }

void VertexBuffer::setLayout(const BufferLayout& bufferLayout) { // bind antes necessario!!! enableVertexAttribArray

    layout = bufferLayout;
    const std::vector<BufferElement>& elements = bufferLayout.getLayout();
    for (uint8_t i = 0; i < elements.size(); i++) {

        const BufferElement& element = elements[i];
        glEnableVertexAttribArray(i);
        if (elements.size() > 1) // FIXME: se componente for diferente de floatprecisa usar outro (ex: glVertexAttribIPointer, para int)
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), BUFFER_OFFSET(element.offset));
        else
            glVertexAttribPointer(i, element.count, element.type, element.normalized, 0, BUFFER_OFFSET(0));
    }
    // for (uint8_t i = 0; i < elements.size(); i++)
    //     glDisableVertexAttribArray(i);
}

void VertexBuffer::setData(const void* data, const uint32_t& size) {
    glBufferData(GL_ARRAY_BUFFER, size * layout.getStride(), data, (GLuint)type);
}

void VertexBuffer::setSubData(const void* data, const uint32_t& offset, const uint32_t& size) {
    glBufferSubData(GL_ARRAY_BUFFER, offset, size * layout.getStride(), data);
}

void VertexBuffer::setSubData2(const void* data, const uint32_t& offset, const uint32_t& size) {
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

// GetPointerInternal //bind anter necessario
void* VertexBuffer::map() { return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); }
// ReleasePointer
void VertexBuffer::unmap() { glUnmapBuffer(GL_ARRAY_BUFFER); }

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }

void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

} // namespace Chimera