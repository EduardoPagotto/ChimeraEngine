#include "chimera/render/vbs/EBO.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/render/vbs/VertexData.hpp"

namespace Chimera {

EBO::EBO() : eboGL(0) {}

EBO::~EBO() {
    if (eboGL > 0)
        glDeleteBuffers(1, &eboGL);
}

void EBO::create(std::vector<unsigned int>& index) {

    unsigned int sizeBufferIndex = index.size() * sizeof(unsigned int);
    sizeIndex = index.size();

    // Create EBO
    glGenBuffers(1, &eboGL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboGL);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &index[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::render() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboGL);
    glDrawElements(GL_TRIANGLES, sizeIndex, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace Chimera