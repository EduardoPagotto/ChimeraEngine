#pragma once
#include "BufferLayout.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

#define BUFFER_OFFSET(i) ((void*)(i))

enum class BufferType { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW, STREAM = GL_STREAM_DRAW };

class VertexBuffer {
  private:
    uint32_t bufferID = 0;
    uint32_t sizeTotal = 0;
    BufferLayout layout;
    BufferType type;

  public:
    VertexBuffer(BufferType type) : type(type) { glGenBuffers(1, &bufferID); }
    VertexBuffer(BufferType type, const uint32_t& size, void* data) {
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, data, (GLuint)type);
        sizeTotal = size;
    }

    virtual ~VertexBuffer() { glDeleteBuffers(1, &bufferID); }

    void reSize(const uint32_t& size) { glBufferData(GL_ARRAY_BUFFER, size * layout.getStride(), nullptr, (GLuint)type); }

    void setLayout(const BufferLayout& bufferLayout) {
        layout = bufferLayout;
        const std::vector<BufferElement>& elements = bufferLayout.getLayout();
        for (uint8_t i = 0; i < elements.size(); i++) {

            const BufferElement& element = elements[i];
            glEnableVertexAttribArray(i);
            if (elements.size() > 1) // FIXME: se componente for diferente de floatprecisa usar outro (ex: glVertexAttribIPointer, para int)
                glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(),
                                      BUFFER_OFFSET(element.offset));
            else
                glVertexAttribPointer(i, element.count, element.type, element.normalized, 0, BUFFER_OFFSET(0));
        }
        // for (uint8_t i = 0; i < elements.size(); i++)
        //     glDisableVertexAttribArray(i);
    }

    inline void setData(const void* data, const uint32_t& size) {
        glBufferData(GL_ARRAY_BUFFER, size * layout.getStride(), data, (GLuint)type);
    }

    inline void setSubData(const void* data, const uint32_t& offset, const uint32_t& size) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size * layout.getStride(), data);
    }

    inline void setSubData2(const void* data, const uint32_t& offset, const uint32_t& size) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    inline void* map() { return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); }

    inline void unmap() { glUnmapBuffer(GL_ARRAY_BUFFER); }
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    static void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }
};
} // namespace Chimera
