#include "chimera/render/HeightMap.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ) {
    vao = new Core::VertexArray();
}

HeightMap::~HeightMap() {
    vao->bind();
    while (vNodes.size() > 0) {
        std::vector<VertexNode*>::iterator it = vNodes.begin();
        VertexNode* pNode = (*it);
        delete pNode;
        pNode = nullptr;
        vNodes.erase(it);
    }
    vao->unbind();

    delete vao;
    vao = nullptr;
}

void HeightMap::split(std::vector<unsigned int> _vVertexIndex) {

    bool done = false;
    uint32_t startHeight = 0;
    uint32_t startWidth = 0;
    uint32_t contador = 0;
    uint32_t totalHeight = (height - 1) * 2;
    uint32_t totalWidth = (width - 1) * 2;
    uint32_t squareHeight = squareZ;
    uint32_t squareWidth = squareX * 2;
    uint32_t totfaces = _vVertexIndex.size() / 3;
    uint32_t thresholdWidht = totalHeight * squareZ;

    while (!done) {

        uint32_t endHeight = startHeight + squareHeight;
        uint32_t endWidth = startWidth + squareWidth;
        uint32_t testeA = startHeight * totalHeight + startWidth;

        if (testeA >= totfaces) {
            done = true;
            continue;
        }

        if (endHeight > (height - 1))
            endHeight = (height - 1);

        if (endWidth > totalWidth)
            endWidth = totalWidth;

        VertexNode* pNode = new VertexNode;

        uint32_t face, base;
        for (uint32_t h = startHeight; h < endHeight; h++) {   // z
            for (uint32_t w = startWidth; w < endWidth; w++) { // x
                face = ((h * totalHeight) + w);
                base = face * 3;
                pNode->addFace(_vVertexIndex[base], _vVertexIndex[base + 1], _vVertexIndex[base + 2]);
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
        // pNode->debugDados();
    }
}

void HeightMap::createVertexBuffer(std::vector<VertexData>& _vertexData) {

    vao->addBuffer(new Core::VertexBuffer(&_vertexData[0], _vertexData.size(), 3), 0); // FIXME: 0 para compatibilidade
    vao->bind();

    for (VertexNode* pNode : vNodes) {
        pNode->initIndexBufferObject(); // create IBO's
        pNode->initAABB(_vertexData);   // initialize AABB's
    }

    vao->unbind();
}

void HeightMap::render(Frustum& _frustrun) {
    vao->bind();
    unsigned int tot = 0;
    for (VertexNode* pNode : vNodes) {
        bool teste = pNode->aabb.visible(_frustrun);
        if (teste == true) {
            pNode->render();
            tot++;
        }
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "TOT: (%d)", tot);
    vao->unbind();
}
} // namespace Chimera