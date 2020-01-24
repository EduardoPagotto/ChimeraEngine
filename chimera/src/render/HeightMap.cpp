#include "chimera/render/HeightMap.hpp"
#include "chimera/OpenGLDefs.hpp"

namespace Chimera {

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ), VertexBuffer(false) {}

HeightMap::~HeightMap() {

    glBindVertexArray(vao);
    while (vNodes.size() > 0) {
        std::vector<VertexNode*>::iterator it = vNodes.begin();
        VertexNode* pNode = (*it);
        delete pNode;
        pNode = nullptr;
        vNodes.erase(it);
    }
}

void HeightMap::split(std::vector<unsigned int> _vVertexIndex) {

    int totalHeight = (height - 1) * 2; // 14
    int totalWidth = (width - 1) * 2;   // 14

    int squareHeight = squareZ; // * 2;
    int squareWidth = squareX * 2;

    int startHeight = 0;
    int startWidth = 0;
    bool done = false;

    int totfaces = _vVertexIndex.size() / 3;

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

        unsigned int face, base;
        for (unsigned int h = startHeight; h < endHeight; h++) {   // z
            for (unsigned int w = startWidth; w < endWidth; w++) { // x
                face = ((h * totalHeight) + w);
                base = face * 3;
                pNode->addFace(face, _vVertexIndex[base], _vVertexIndex[base + 1], _vVertexIndex[base + 2]);
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
            // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "shit!!!");
            continue;
        }

        vNodes.push_back(pNode);
        pNode->debugDados();
    }
}

void HeightMap::createIndex() {

    for (VertexNode* pNode : vNodes) {
        pNode->initIndexBufferObject(); // create IBO's
    }
}

void HeightMap::clearIndex() {

    // vincula ibo's
    for (VertexNode* pNode : vNodes) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pNode->getIndexBufferObject());
    }
}

void HeightMap::createVertexBuffer(std::vector<VertexData>& _vertexData) {

    VertexBuffer::initialize(_vertexData);

    for (VertexNode* pNode : vNodes) {
        pNode->initAABB(_vertexData); // initialize AABB's
    }
}

void HeightMap::render() {
    glBindVertexArray(vao);
    for (VertexNode* pNode : vNodes) {
        pNode->render();
    }
    glBindVertexArray(0);
}

} // namespace Chimera