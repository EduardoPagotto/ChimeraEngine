#include "chimera/render/HeightMap.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ) {}

HeightMap::~HeightMap() {}

void HeightMap::chunkSquare(std::vector<Triangle>& _tris) {}

void HeightMap::split(MeshData& _mesh) {

    int totalHeight = (height - 1) * 2; // 14
    int totalWidth = (width - 1) * 2;   // 14

    int squareHeight = squareZ; // * 2;
    int squareWidth = squareX * 2;

    int startHeight = 0;
    int startWidth = 0;
    bool done = false;

    std::vector<Triangle> triangles;
    std::vector<unsigned int> indexTriangles;
    _mesh.toTriangle(triangles, indexTriangles);

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

                int indexA = (h * totalHeight) + w;
                unsigned indexT = indexTriangles[indexA];
                Triangle t = triangles[indexT];

                pNode->triangles.push_back(t);
                pNode->index.push_back(indexA);

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

        if (pNode->index.size() == 0) {
            delete pNode;
            pNode = nullptr;
            done = true;
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "shit!!!");
            continue;
        }

        vNodes.push_back(pNode);

        std::string val = "Index :";
        for (int i = 0; i < pNode->index.size(); i++) {
            val.append(" " + std::to_string(pNode->index[i]));
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "%s", val.c_str());
    }
}

} // namespace Chimera