#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera::Core {

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

void VertexBuffer::reSize(const uint32_t& size) { glBufferData(GL_ARRAY_BUFFER, size * layout->sizeVertex, nullptr, getBufferType(type)); }

void VertexBuffer::setLayout(VertexComponents* layout) { // bind antes necessario!!! enableVertexAttribArray

    this->layout = layout;
    for (auto slot : layout->loc) {
        glEnableVertexAttribArray(slot.id); // fixme trocar por array e remover slot.id usando indice do array no lugar

        if (layout->loc.size() > 1)
            glVertexAttribPointer(slot.id, slot.size, slot.type, slot.normalize, layout->sizeVertex, slot.offset);
        else
            glVertexAttribPointer(slot.id, slot.size, slot.type, slot.normalize, 0, BUFFER_OFFSET(0));
    }
}

// void VertexBuffer::releaseAtributes() { // no lugar de disableVertexAttribArray
//     for (auto slot : layout->loc)
//         glDisableVertexAttribArray(slot.id);
// }

void VertexBuffer::setData(const void* data, const uint32_t& size) {
    glBufferData(GL_ARRAY_BUFFER, size * layout->sizeVertex, data, getBufferType(type));
}

// GetPointerInternal //bind anter necessario
void* VertexBuffer::map() { return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); }
// ReleasePointer
void VertexBuffer::unmap() { glUnmapBuffer(GL_ARRAY_BUFFER); }
void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
} // namespace Chimera::Core