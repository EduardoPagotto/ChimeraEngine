#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/OpenGLDefs.hpp"

namespace Chimera {

VertexArray::VertexArray() { glGenVertexArrays(1, &arrayID); }

VertexArray::~VertexArray() {

    for (int i = 0; i < vbos.size(); i++)
        delete vbos[i];

    glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::push(VertexBuffer* buffer) { this->vbos.push_back(buffer); }

void VertexArray::bind() const { glBindVertexArray(arrayID); }

void VertexArray::unbind() { glBindVertexArray(0); }

} // namespace Chimera