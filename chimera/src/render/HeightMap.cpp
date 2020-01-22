#include "chimera/render/HeightMap.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

NodeHeightMap::NodeHeightMap() { indexBufferObject = 0; }
NodeHeightMap::~NodeHeightMap() {}

void NodeHeightMap::debugDados() {

    std::string val = "Index :";
    for (int i = 0; i < index.size(); i++)
        val.append(" " + std::to_string(index[i]));

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "%s", val.c_str());
}

void NodeHeightMap::initAABB(std::vector<glm::vec3> _vertexListMesh, std::vector<unsigned int> _vertexIndexMesh) {

    std::vector<glm::vec3> vlt;

    for (unsigned int indexFace : index) {

        unsigned int A = indexFace * 3;
        unsigned int B = A + 1;
        unsigned int C = A + 2;

        unsigned int iA = _vertexIndexMesh[A];
        unsigned int iB = _vertexIndexMesh[B];
        unsigned int iC = _vertexIndexMesh[C];

        glm::vec3 pA = _vertexListMesh[iA];
        glm::vec3 pB = _vertexListMesh[iB];
        glm::vec3 pC = _vertexListMesh[iC];

        vlt.push_back(pA);
        vlt.push_back(pB);
        vlt.push_back(pC);
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

void NodeHeightMap::initIndexBufferObject() {

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // delete [] Indices;
    // Indices = NULL;
    // GeometryNodesCount++;
}

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ) {}

HeightMap::~HeightMap() {}

void HeightMap::split(MeshData& _mesh) {

    int totalHeight = (height - 1) * 2; // 14
    int totalWidth = (width - 1) * 2;   // 14

    int squareHeight = squareZ; // * 2;
    int squareWidth = squareX * 2;

    int startHeight = 0;
    int startWidth = 0;
    bool done = false;

    std::vector<unsigned int> indexTriangles;
    _mesh.faceIndex(indexTriangles);

    int contador = 0;
    int thresholdWidht = totalHeight * squareZ;

    while (!done) {

        int endHeight = startHeight + squareHeight;
        int endWidth = startWidth + squareWidth;

        int testeA = startHeight * totalHeight + startWidth;
        int testeB = indexTriangles.size();
        if (testeA >= testeB) {
            done = true;
            continue;
        }

        if (endHeight > (height - 1))
            endHeight = (height - 1);

        if (endWidth > totalWidth)
            endWidth = totalWidth;

        NodeHeightMap* pNode = new NodeHeightMap;

        for (int h = startHeight; h < endHeight; h++) {   // z
            for (int w = startWidth; w < endWidth; w++) { // x
                unsigned int indexA = (h * totalHeight) + w;
                pNode->addIndex(indexA);
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
} // namespace Chimera