
#include "chimera/render/VertexNode.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

VertexNode::VertexNode() { indexBufferObject = 0; }

VertexNode::~VertexNode() {
    if (indexBufferObject > 0)
        glDeleteBuffers(1, &indexBufferObject);

    faces.clear();
}

void VertexNode::debugDados() {
    std::string val = "";
    for (int i = 0; i < faces.size(); i++)
        val.append(std::to_string(faces[i]) + " ");

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Faces (%ld): %s", faces.size(), val.c_str());
}

void VertexNode::initAABB(std::vector<VertexData>& vertexDataIn) {

    std::vector<glm::vec3> vlt;

    for (unsigned int indexFace : index) {
        VertexData vertex = vertexDataIn[indexFace];
        vlt.push_back(vertex.position);
    }

    glm::vec3 min, max;
    if (vlt.size() > 0) {
        min = vlt[0];
        max = vlt[0];
    }

    for (unsigned int i = 1; i < vlt.size(); i++) {
        min = glm::min(min, vlt[i]);
        max = glm::max(max, vlt[i]);
    }

    aabb.setBoundary(min, max);

    vlt.clear();
}

void VertexNode::addFace(unsigned int& _face, unsigned int& _pa, unsigned int& _pb, unsigned int& _pc) {
    faces.push_back(_face);
    index.push_back(_pa);
    index.push_back(_pb);
    index.push_back(_pc);
}

void VertexNode::initIndexBufferObject() {

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // delete [] Indices;
    // Indices = NULL;
    // GeometryNodesCount++;
}

void VertexNode::render() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace Chimera