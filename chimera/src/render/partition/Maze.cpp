#include "chimera/render/partition/Maze.hpp"
#include "chimera/core/Exception.hpp"
#include <string>

namespace Chimera {

Maze::Maze(const char filename[]) : sizeBlock(10.0f), indexPointCount(0) {

    char buffer[1024] = {0};

    FILE* file = fopen(filename, "rb");
    if (!file)
        throw Exception(std::string("Arguivo nao localizado"));

    // tamanho do mapa
    fgets(buffer, 1024, file);
    this->size.x = atoi(buffer);

    fgets(buffer, 1024, file);
    this->size.z = atoi(buffer);

    fgets(buffer, 1024, file);
    this->size.y = atoi(buffer);

    glm::ivec3 pos(0);

    this->halfBlock.x = (this->size.x * sizeBlock) / 2.0f; //(w/2)
    this->halfBlock.y = (this->size.y * sizeBlock) / 2.0f; //(d/2)
    this->halfBlock.z = (this->size.z * sizeBlock) / 2.0f; //(h/2)

    // carregando mapa
    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            fgets(buffer, 1024, file);
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {
                glm::vec3 min = this->minimal(pos);
                glm::vec3 max = min + sizeBlock;
                Cube* pCube = new Cube(buffer[pos.x], min, max);
                this->vpCube.push_back(pCube);
            }
        }
    }

    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {
                Cube* pCube = this->vpCube[this->getIndexArrayPos(pos)];

                Cube* pBefore = getCubeNeighbor(DEEP::BOTTOM, CARDINAL::NONE, pos);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::BOTTOM, CARDINAL::NONE, pBefore);

                pBefore = getCubeNeighbor(DEEP::UP, CARDINAL::NONE, pos);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::UP, CARDINAL::NONE, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, pos);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::EAST, pos);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::EAST, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, pos);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::WEST, pos);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::WEST, pBefore);
            }
        }
    }
}

Maze::~Maze() { this->vpCube.clear(); }

glm::vec3 Maze::minimal(const glm::ivec3& pos) const {
    float x_min = (pos.x * sizeBlock) - halfBlock.x; // (width++)
    float y_min = (pos.y * sizeBlock) - halfBlock.y; // Altura Minima andar
    float z_min = (pos.z * sizeBlock) - halfBlock.z; // (height--)
    return glm::vec3(x_min, y_min, z_min);
}

glm::ivec3 Maze::getCardinalPos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos) {
    glm::ivec3 val = pos;
    switch (deep) {
        case DEEP::UP:
            val.y = pos.y + dist.y;
            break;
        case DEEP::MIDDLE:
            val.y = pos.y;
            break;
        case DEEP::BOTTOM:
            val.y = pos.y - dist.y;
        default:
            break;
    }

    switch (card) {
        case CARDINAL::NORTH:
            val.z = pos.z - dist.z;
            break;
        case CARDINAL::NORTH_EAST:
            val.z = pos.z - dist.z;
            val.x = pos.x + dist.x;
            break;
        case CARDINAL::EAST:
            val.x = pos.x + dist.x;
            break;
        case CARDINAL::SOUTH_EAST:
            val.z = pos.z + dist.z;
            val.x = pos.x + dist.x;
            break;
        case CARDINAL::SOUTH:
            val.z = pos.z + dist.z;
            break;
        case CARDINAL::SOUTH_WEST:
            val.z = pos.z + dist.z;
            val.x = pos.x - dist.x;
            break;
        case CARDINAL::WEST:
            val.x = pos.x - dist.x;
            break;
        case CARDINAL::NORTH_WEST:
            val.z = pos.z - dist.z;
            val.x = pos.x - dist.x;
            break;
        default:
            break;
    }
    return val;
}

Cube* Maze::getCubeNeighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos) {
    glm::ivec3 val = this->getCardinalPos(deep, card, glm::ivec3(1), pos);
    if (this->valid(val))
        return this->vpCube[this->getIndexArrayPos(val)];

    return nullptr;
}

void Maze::createMap() {
    // carrega posicoes, texturas, e seq textura defaults do cubo
    initCubeBase();
    for (auto pCube : this->vpCube) {
        pCube->create(&vertexData, &this->trisList);
    }

    for (Triangle t : this->trisList) {
        vIndex.push_back(t.p[0]);
        vIndex.push_back(t.p[1]);
        vIndex.push_back(t.p[2]);
    }
    // limpa dados de criacao do cubo base
    cleanupCubeBase();
    // limpas cubos de contrucao e vetor de cubos
    for (auto pCube : this->vpCube) {
        delete pCube;
        pCube = nullptr;
    }
    vpCube.clear();
}
} // namespace Chimera