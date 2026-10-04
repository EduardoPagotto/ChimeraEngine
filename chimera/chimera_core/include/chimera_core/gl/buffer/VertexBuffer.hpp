#pragma once
#include "BufferLayout.hpp"

namespace ce {

#define BUFFER_OFFSET(i) ((void*)(i))

    enum class BufferType { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW, STREAM = GL_STREAM_DRAW };

    class VertexBuffer {
      public:
        VertexBuffer(BufferType type) : type_(type) { glGenBuffers(1, &buffer_id_); }
        VertexBuffer(BufferType type, const uint32_t& size, void* data) {
            glGenBuffers(1, &buffer_id_);
            glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
            glBufferData(GL_ARRAY_BUFFER, size, data, (GLuint)type);
            size_total_ = size;
        }

        virtual ~VertexBuffer() { glDeleteBuffers(1, &buffer_id_); }

        void re_size(const uint32_t& size) {
            glBufferData(GL_ARRAY_BUFFER, size * layout_.get_stride(), nullptr, (GLuint)type_);
        }

        void set_layout(const BufferLayout& buffer_layout) {
            layout_ = buffer_layout;
            const std::vector<BufferElement>& elements = buffer_layout.get_layout();
            for (uint8_t i = 0; i < elements.size(); i++) {

                const BufferElement& element = elements[i];
                glEnableVertexAttribArray(i);
                if (elements.size() > 1) // FIXME: se componente for diferente de floatprecisa usar outro (ex:
                                         // glVertexAttribIPointer, para int)
                    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout_.get_stride(),
                                          BUFFER_OFFSET(element.offset));
                else
                    glVertexAttribPointer(i, element.count, element.type, element.normalized, 0, BUFFER_OFFSET(0));
            }
            // for (uint8_t i = 0; i < elements.size(); i++)
            //     glDisableVertexAttribArray(i);
        }

        inline void set_data(const void* data, const uint32_t& size) {
            glBufferData(GL_ARRAY_BUFFER, size * layout_.get_stride(), data, (GLuint)type_);
        }

        inline void set_sub_data(const void* data, const uint32_t& offset, const uint32_t& size) {
            glBufferSubData(GL_ARRAY_BUFFER, offset, size * layout_.get_stride(), data);
        }

        inline void set_sub_data2(const void* data, const uint32_t& offset, const uint32_t& size) {
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        }

        inline void* map() { return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); }

        inline void unmap() { glUnmapBuffer(GL_ARRAY_BUFFER); }
        inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, buffer_id_); }
        static void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

        // se nao existir IBO new rander pelo VAO
        // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }

      private:
        uint32_t buffer_id_ = 0;
        uint32_t size_total_ = 0;
        BufferLayout layout_;
        BufferType type_;
    };
} // namespace ce
