#include "chimera/render/vbs/VAO.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

VAO::VAO() : vaoGL(0) {}

VAO::~VAO() {
    if (vaoGL > 0)
        glDeleteVertexArrays(1, &vaoGL);
}

void VAO::create() { glGenVertexArrays(1, &vaoGL); }

void VAO::bind() { glBindVertexArray(vaoGL); }

void VAO::unbind() { glBindVertexArray(0); }

} // namespace Chimera