#include "chimera/render/VAO.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

VAO::VAO() : vao(0) {}

VAO::~VAO() {
    if (vao > 0)
        glDeleteVertexArrays(1, &vao);
}

void VAO::create() { glGenVertexArrays(1, &vao); }

void VAO::bind() { glBindVertexArray(vao); }

void VAO::unbind() { glBindVertexArray(0); }

} // namespace Chimera