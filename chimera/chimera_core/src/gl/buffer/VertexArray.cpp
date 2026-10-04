#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"

namespace ce {

    VertexArray::VertexArray() { glGenVertexArrays(1, &array_id_); }

    VertexArray::~VertexArray() {

        for (int i = 0; i < vbos_.size(); i++)
            vbos_[i].reset();

        vbos_.clear();

        glDeleteVertexArrays(1, &array_id_);
    }

    void VertexArray::bind() const { glBindVertexArray(array_id_); }
    void VertexArray::unbind() { glBindVertexArray(0); }

} // namespace ce
