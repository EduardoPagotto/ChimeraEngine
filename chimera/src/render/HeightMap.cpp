#include "chimera/render/HeightMap.hpp"

namespace Chimera {

HeightMap::HeightMap(int _width, int _height, int _squareX, int _squareZ)
    : width(_width), height(_height), squareX(_squareX), squareZ(_squareZ) {}

HeightMap::~HeightMap() {}

void HeightMap::chunkSquare(std::vector<Triangle>& _tris) {}

void HeightMap::split(MeshData& _mesh) {

    int tot_z = (height - 1) * 2; // 14
    int tot_x = (width - 1) * 2;  // 14

    int squareZT = squareZ * 6;
    int squareXT = squareX * 6;

    int sz = 0;
    int sx = 0;
    bool done = false;

    std::vector<glm::vec3> vertexList = _mesh.getVertexList();
    std::vector<unsigned int> vertexIndex = _mesh.getVertexIndex();

    while (!done) {

        int fz = sz + squareZT;
        int fx = sx + squareXT;

        NodeHeightMap* pNode = new NodeHeightMap;

        for (int z = sz; z < fz; z++) {
            for (int x = sx; x < fx; x++) {

                // if ((x == tot_x) || (z == tot_z))
                //     continue;

                int indiceT = (z * tot_z) + x;
                int index = vertexIndex[indiceT];
                pNode->mesh.addVertexIndex(index);
                pNode->mesh.addVertex(vertexList[index]);
            }
        }

        sz = fz;
        sx = fx;

        vNodes.push_back(pNode);

        done = (fx >= tot_x) && (fz >= tot_z);
    }
}

} // namespace Chimera