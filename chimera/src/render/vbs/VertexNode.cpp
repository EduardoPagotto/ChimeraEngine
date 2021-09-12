
#include "chimera/render/vbs/VertexNode.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

VertexNode::VertexNode() : ibo(nullptr) {}

VertexNode::~VertexNode() {
    if (ibo != nullptr) {
        delete ibo;
        ibo = nullptr;
    }
}

void VertexNode::debugDados() {
    glm::vec3 pos = this->aabb.getPosition();
    glm::vec3 size = this->aabb.getSize();

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
}

void VertexNode::initAABB(std::vector<VertexData>& vertexDataIn) {
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(&vertexDataIn[0], vertexDataIn.size(), &indexTris[0], indexTris.size(), min, max, size);
    aabb.setBoundary(min, max);
}

void VertexNode::addFace(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
    indexTris.push_back(_pa);
    indexTris.push_back(_pb);
    indexTris.push_back(_pc);
}

void VertexNode::initIndexBufferObject() { ibo = new Core::IndexBuffer(&indexTris[0], indexTris.size()); }

void VertexNode::render() {
    // ibo->render();
    ibo->bind();
    glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    ibo->unbind();
}
} // namespace Chimera