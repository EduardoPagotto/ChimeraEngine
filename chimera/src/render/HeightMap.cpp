#include "chimera/render/HeightMap.hpp"

namespace Chimera {

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ) {}

HeightMap::~HeightMap() {}

void HeightMap::chunkSquare(std::vector<Triangle>& _tris) {}

void HeightMap::split(MeshData& _mesh) {

    int totalHeight = (height - 1) * 2; // 14
    int totalWidth = (width - 1) * 2;   // 14

    int squareHeight = squareZ * 2;
    int squareWidth = squareX * 2;

    int startHeight = 0;
    int startWidth = 0;
    bool done = false;

    std::vector<Triangle> triangles;
    _mesh.toTriangle(triangles);
    std::vector<unsigned int> indexTriangles = _mesh.getVertexIndex();

    while (!done) {

        int endHeight = startHeight + squareHeight;
        int endWidth = startWidth + squareWidth;

        NodeHeightMap* pNode = new NodeHeightMap;

        for (int h = startHeight; h < endHeight; h++) {   // z
            for (int w = startWidth; w < endWidth; w++) { // x

                if ((w == totalWidth) || (h == totalHeight))
                    continue;

                int indexA = (h * totalHeight) + w;
                int indexT = indexTriangles[index];

                pNode->triangles.push_back(indexT);
            }
        }

        startHeight = endHeight;
        startWidth = endWidth;

        vNodes.push_back(pNode);

        done = (endWidth >= totalWidth) && (endHeight >= totalHeight);
    }
}

} // namespace Chimera