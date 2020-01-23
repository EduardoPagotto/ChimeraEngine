#include "chimera/render/HeightMap.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ) {
    vao = 0;
    vbo = 0;
}

HeightMap::~HeightMap() {
    if (vao > 0)
        glDeleteVertexArrays(1, &vao);

    if (vbo > 0)
        glDeleteBuffers(1, &vbo);
}

void HeightMap::split(MeshData& _mesh) {

    int totalHeight = (height - 1) * 2; // 14
    int totalWidth = (width - 1) * 2;   // 14

    int squareHeight = squareZ; // * 2;
    int squareWidth = squareX * 2;

    int startHeight = 0;
    int startWidth = 0;
    bool done = false;

    int totfaces = _mesh.getVertexIndex().size() / 3;

    int contador = 0;
    int thresholdWidht = totalHeight * squareZ;

    while (!done) {

        int endHeight = startHeight + squareHeight;
        int endWidth = startWidth + squareWidth;

        int testeA = startHeight * totalHeight + startWidth;
        if (testeA >= totfaces) {
            done = true;
            continue;
        }

        if (endHeight > (height - 1))
            endHeight = (height - 1);

        if (endWidth > totalWidth)
            endWidth = totalWidth;

        VertexNode* pNode = new VertexNode;

        std::vector<unsigned int> vIndex = _mesh.getVertexIndex();

        for (int h = startHeight; h < endHeight; h++) {   // z
            for (int w = startWidth; w < endWidth; w++) { // x
                unsigned int indexA = (h * totalHeight) + w;

                int A = indexA * 3;
                int B = A + 1;
                int C = B + 1;

                int pa = vIndex[A];
                int pb = vIndex[B];
                int pc = vIndex[C];

                pNode->addIndex(pa);
                pNode->addIndex(pb);
                pNode->addIndex(pc);

                // pNode->addIndex(indexA); indexA é o numero da face
                contador++;
            }
        }

        if (contador >= thresholdWidht) {
            startHeight = endHeight;
            contador = 0;
            startWidth = 0;
        } else {
            startWidth = endWidth;
        }

        if (pNode->empty() == true) {
            delete pNode;
            pNode = nullptr;
            done = true;
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "shit!!!");
            continue;
        }

        vNodes.push_back(pNode);

        pNode->debugDados();
        pNode->initAABB(_mesh.getVertexList(), _mesh.getVertexIndex());
    }
}

void HeightMap::createVertexBuffer(std::vector<VertexData>& _vertexData) {

    // cria o vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Buffer de vertice
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.size() * sizeof(VertexData), &_vertexData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (VertexNode* pNode : vNodes) {
        pNode->initIndexBufferObject();
    }

    // vincula vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

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

    // vincula ibo's
    for (VertexNode* pNode : vNodes) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pNode->getIndexBufferObject());
    }

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(positionID);
    glDisableVertexAttribArray(normalID);
    glDisableVertexAttribArray(uvID);
}

void HeightMap::render() {
    glBindVertexArray(vao);
    for (VertexNode* pNode : vNodes) {
        pNode->render();
    }
    glBindVertexArray(0);
}

} // namespace Chimera