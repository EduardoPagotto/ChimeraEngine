#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/gl/OpenGLDefs.hpp"

namespace ce {

VertexArray::VertexArray() { glGenVertexArrays(1, &arrayID); }

VertexArray::~VertexArray() {

    for (int i = 0; i < vbos.size(); i++)
        delete vbos[i];

    vbos.clear();

    glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::bind() const { glBindVertexArray(arrayID); }

void VertexArray::unbind() { glBindVertexArray(0); }

} // namespace ce