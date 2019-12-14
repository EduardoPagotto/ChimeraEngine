#include "chimera/core/VertexStatic.hpp"
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

VertexRenderStatic::VertexRenderStatic() {
    VertexVBOID = 0;
    IndexVBOID = 0;
    VAO = 0;
}
VertexRenderStatic::~VertexRenderStatic() {
    glDeleteBuffers(1, &VertexVBOID);
    glDeleteBuffers(1, &IndexVBOID);
}

void VertexRenderStatic::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
}

void VertexRenderStatic::create(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index) {

    if (index.size() == 0) {
        indexVBO_slow(vertexDataIn, vertexData, indexIBO);
    } else {
        vertexData = vertexDataIn;
        indexIBO = index;
    }

    unsigned int sizeBufferVertex = vertexData.size() * sizeof(VertexData);
    unsigned int sizeBufferIndex = indexIBO.size() * sizeof(unsigned int);

    // Buffer de vertice
    glGenBuffers(1, &VertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeBufferVertex, &vertexData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Buffer de indice
    glGenBuffers(1, &IndexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &indexIBO[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // cria o VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vincula VBO
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);

    // Vertice
    GLuint positionID = 0; // glGetAttribLocation(shader.getIdProgram(), "position");
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(positionID);

    // Normal
    GLuint normalID = 1; // glGetAttribLocation(shader.getIdProgram(), "normal");
    glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(normalID);

    // Texture
    GLuint uvID = 2; // glGetAttribLocation(shader.getIdProgram(), "uv1");
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(24));
    glEnableVertexAttribArray(uvID);

    // vincula IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(positionID);
    glDisableVertexAttribArray(normalID);
    glDisableVertexAttribArray(uvID);
}
} // namespace Chimera