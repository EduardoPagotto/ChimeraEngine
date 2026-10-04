#include "chimera_core/gl/buffer/IndexBuffer.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"

namespace ce {

    IndexBuffer::IndexBuffer(uint32_t* data, const uint32_t& size) : size_(size) {
        // Create IndexBuffer
        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &buffer_id_); }

    void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_); }

    void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace ce
