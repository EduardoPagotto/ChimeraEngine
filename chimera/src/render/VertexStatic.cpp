#include "chimera/render/VertexStatic.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2) { return fabs(v1 - v2) < EPSILON; } // 0.01f

bool getSimilarVertexIndex(VertexData& in_vertex, std::vector<VertexData>& out_vertex, unsigned int& result) {
    // Percorrer todos os vertex ja existentes na lista
    for (unsigned int i = 0; i < out_vertex.size(); i++) {

        if (is_near(in_vertex.position.x, out_vertex[i].position.x) &&
            is_near(in_vertex.position.y, out_vertex[i].position.y) &&
            is_near(in_vertex.position.z, out_vertex[i].position.z) &&

            is_near(in_vertex.normal.x, out_vertex[i].normal.x) &&
            is_near(in_vertex.normal.y, out_vertex[i].normal.y) &&
            is_near(in_vertex.normal.z, out_vertex[i].normal.z) &&

            is_near(in_vertex.texture.x, out_vertex[i].texture.x) &&
            is_near(in_vertex.texture.y, out_vertex[i].texture.y)) {
            result = i;
            return true;
        }
    }
    return false;
}

void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
                   std::vector<unsigned int>& out_indices) {

    // percorrer todos os vertices
    for (unsigned int i = 0; i < inData.size(); i++) {
        // Procura por similar
        unsigned int index;
        if (getSimilarVertexIndex(inData[i], outData, index) == true) {
            // se entrotar usar apenas o indice
            out_indices.push_back(index);

        } else {
            // se nao adiciona a lista de novo vertex
            outData.push_back(inData[i]);
            out_indices.push_back((unsigned int)outData.size() - 1);
        }
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "VBO Vertex In: %04lu Vertex out: %04lu Index out: %04lu ", inData.size(),
                 outData.size(), out_indices.size());
}

VertexRenderStatic::VertexRenderStatic() : VBO() { ibo = 0; }
VertexRenderStatic::~VertexRenderStatic() {

    if (vertexData.size() > 0)
        vertexData.clear();

    if (indexIBO.size() > 0)
        indexIBO.clear();

    if (ibo > 0)
        glDeleteBuffers(1, &ibo);
}

void VertexRenderStatic::createIndex() {

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &indexIBO[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexRenderStatic::clearIndex() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); }

void VertexRenderStatic::render() {
    vao.bind(); // glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    vao.unbind(); // glBindVertexArray(0);
}

void VertexRenderStatic::create(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index) {

    if (index.size() == 0) {
        indexVBO_slow(vertexDataIn, vertexData, indexIBO);
    } else {
        vertexData = vertexDataIn;
        indexIBO = index;
    }

    sizeBufferIndex = indexIBO.size() * sizeof(unsigned int);

    VBO::initialize(vertexData, false);
}
} // namespace Chimera