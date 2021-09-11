
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
    // std::string val = "";
    // for (int i = 0; i < faces.size(); i++)
    //     val.append(std::to_string(faces[i]) + " ");

    // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Faces (%ld): %s", faces.size(), val.c_str());

    glm::vec3 pos = this->aabb.getPosition();
    glm::vec3 size = this->aabb.getSize();

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
}

void VertexNode::initAABB(std::vector<VertexData>& vertexDataIn) {

    std::vector<glm::vec3> vlt;

    for (uint32_t indexFace : index) {
        VertexData vertex = vertexDataIn[indexFace];
        vlt.push_back(vertex.position);
    }

    glm::vec3 min, max;
    if (vlt.size() > 0) {
        min = vlt[0];
        max = vlt[0];
    }

    for (uint32_t i = 1; i < vlt.size(); i++) {
        min = glm::min(min, vlt[i]);
        max = glm::max(max, vlt[i]);
    }

    aabb.setBoundary(min, max);

    vlt.clear();
}

void VertexNode::addFace(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
    index.push_back(_pa);
    index.push_back(_pb);
    index.push_back(_pc);
}

void VertexNode::initIndexBufferObject() {

    ibo = new Core::IndexBuffer(&index[0], index.size());
    // delete [] Indices; //FIXME: verificar como!!!!
    // Indices = NULL;
    // GeometryNodesCount++;
}

void VertexNode::render() { ibo->render(); }
} // namespace Chimera