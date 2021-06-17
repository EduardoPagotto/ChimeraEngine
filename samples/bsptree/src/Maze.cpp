#include "Maze.hpp"
#include "chimera/core/Exception.hpp"
#include <string>

Maze::Maze(const char filename[]) {

    FILE* file;
    char string[1024];

    indexPointCount = 0;

    file = fopen(filename, "rb");
    if (!file) {
        throw Chimera::Exception(std::string("Arguivo nao localizado"));
    }

    // tamanho do mapa
    fgets(string, 1024, file);
    this->size.x = atoi(string);

    fgets(string, 1024, file);
    this->size.z = atoi(string);

    fgets(string, 1024, file);
    this->size.y = atoi(string);

    glm::ivec3 pos(0);

    this->sizeBlock = 10.0f;
    this->halfBlock.x = (this->size.x * sizeBlock) / 2.0f; //(w/2)
    this->halfBlock.y = (this->size.y * sizeBlock) / 2.0f; //(d/2)
    this->halfBlock.z = (this->size.z * sizeBlock) / 2.0f; //(h/2)

    // carregando mapa
    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            fgets(string, 1024, file);
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {
                glm::vec3 min = this->minimal(pos);
                glm::vec3 max = min + sizeBlock;
                Cube* pCube = new Cube(string[pos.x], pos, min, max);
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

// // lembrar d = y;  w = x;  h = z;
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

// // lembrar d = y;  w = x;  h = z;
SPACE Maze::getCardinalNeighbor(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos) {
    glm::ivec3 val = this->getCardinalPos(deep, card, dist, pos);
    if (this->valid(val))
        return this->getCardinal(val);

    return SPACE::INVALID;
}

Cube* Maze::getCubeNeighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos) {
    glm::ivec3 val = this->getCardinalPos(deep, card, glm::ivec3(1), pos);
    if (this->valid(val))
        return this->vpCube[this->getIndexArrayPos(val)];

    return nullptr;
}

bool Maze::hasNeighbor(CARDINAL card, SPACE space, const glm::ivec3& pos) {
    bool val = false;

    if (card == CARDINAL::NORTH_EAST) {
        val = (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos) == space) &&
              (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos) == space);

    } else if (card == CARDINAL::SOUTH_EAST) {
        val = (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos) == space) &&
              (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos) == space);
    }

    else if (card == CARDINAL::SOUTH_WEST) {
        val = (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos) == space) &&
              (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos) == space);

    } else if (card == CARDINAL::NORTH_WEST) {
        val = (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos) == space) &&
              (this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos) == space);
    }

    return val;
}

// CARDINAL Maze::emptyQuadrantDiag(bool invert, const glm::ivec3& pos) {

//     bool isN = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos));
//     bool isE = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos));
//     bool isS = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos));
//     bool isW = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos));

//     if (isN && isE)
//         return (!invert) ? CARDINAL::NORTH_EAST : CARDINAL::SOUTH_WEST;

//     if (isS && isE)
//         return (!invert) ? CARDINAL::SOUTH_EAST : CARDINAL::NORTH_WEST;

//     if (isS && isW)
//         return (!invert) ? CARDINAL::SOUTH_WEST : CARDINAL::NORTH_EAST;

//     if (isN && isW)
//         return (!invert) ? CARDINAL::NORTH_WEST : CARDINAL::SOUTH_EAST;

//     return CARDINAL::NONE;
// }

// // lembrar d = y;  w = x;  h = z;
void Maze::createMap() {
    glm::ivec3 pos;
    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {

                SPACE val = this->getCardinal(pos);
                Cube* pCube = this->vpCube[this->getIndexArrayPos(pos)];
                switch (val) {
                    case SPACE::EMPTY:
                        pCube->newWall(vertexData, this->trisList);
                        break;
                    case SPACE::FLOOR: {
                        pCube->newWall(vertexData, this->trisList);
                        if (SPACE::DIAG == this->getCardinalNeighbor(DEEP::BOTTOM, CARDINAL::NONE, glm::ivec3(1), pos)) {
                            CARDINAL normDiag = pCube->emptyQuadrantDiag(DEEP::BOTTOM, true);
                            // this->emptyQuadrantDiag(true, this->getCardinalPos(DEEP::BOTTOM, CARDINAL::NONE, glm::ivec3(1), pos));
                            pCube->newFlatFloorCeeling(true, normDiag, vertexData, this->trisList);
                        } else {
                            pCube->newFlatFloorCeeling(true, CARDINAL::NONE, vertexData, this->trisList);
                        }
                    } break;
                    case SPACE::CEILING: {
                        pCube->newWall(vertexData, this->trisList);
                        if (SPACE::DIAG == this->getCardinalNeighbor(DEEP::UP, CARDINAL::NONE, glm::ivec3(1), pos)) {
                            CARDINAL normDiag = pCube->emptyQuadrantDiag(DEEP::UP, true);
                            //   this->emptyQuadrantDiag(true, this->getCardinalPos(DEEP::UP, CARDINAL::NONE, glm::ivec3(1), pos));
                            pCube->newFlatFloorCeeling(false, normDiag, vertexData, this->trisList);
                        } else {
                            pCube->newFlatFloorCeeling(false, CARDINAL::NONE, vertexData, this->trisList);
                        }
                    } break;
                    case SPACE::FC: {
                        pCube->newWall(vertexData, this->trisList);
                        pCube->newFlatFloorCeeling(true, CARDINAL::NONE, vertexData, this->trisList);
                        pCube->newFlatFloorCeeling(false, CARDINAL::NONE, vertexData, this->trisList);
                    } break;
                    case SPACE::DIAG: {
                        CARDINAL normDiag = pCube->emptyQuadrantDiag(DEEP::MIDDLE, false);
                        pCube->newDiag(normDiag, vertexData, this->trisList);

                        if (this->hasNeighbor(normDiag, SPACE::FLOOR, pos) == true)
                            pCube->newFlatFloorCeeling(true, normDiag, vertexData, this->trisList);

                        if (this->hasNeighbor(normDiag, SPACE::CEILING, pos) == true)
                            pCube->newFlatFloorCeeling(false, normDiag, vertexData, this->trisList);

                        if (this->hasNeighbor(normDiag, SPACE::FC, pos) == true) {
                            pCube->newFlatFloorCeeling(true, normDiag, vertexData, this->trisList);
                            pCube->newFlatFloorCeeling(false, normDiag, vertexData, this->trisList);
                        }

                    } break;
                    case SPACE::RAMP: {
                        bool isN = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos));
                        bool isE = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos));
                        bool isS = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos));
                        bool isW = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos));

                        if (isN && (!isS)) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                pCube->newRamp(true, CARDINAL::SOUTH, this->vertexData, this->trisList);
                            else if (vazio == SPACE::CEILING)
                                pCube->newRamp(false, CARDINAL::SOUTH, this->vertexData, this->trisList);

                        } else if (isE && (!isW)) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                pCube->newRamp(true, CARDINAL::WEST, this->vertexData, this->trisList);
                            else if (vazio == SPACE::CEILING)
                                pCube->newRamp(false, CARDINAL::WEST, this->vertexData, this->trisList);

                        } else if ((!isN) && isS) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                pCube->newRamp(true, CARDINAL::NORTH, this->vertexData, this->trisList);
                            else if (vazio == SPACE::CEILING)
                                pCube->newRamp(false, CARDINAL::NORTH, this->vertexData, this->trisList);

                        } else if ((!isE) && isW) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                pCube->newRamp(true, CARDINAL::EAST, this->vertexData, this->trisList);
                            else if (vazio == SPACE::CEILING)
                                pCube->newRamp(false, CARDINAL::EAST, this->vertexData, this->trisList);
                        }

                    } break;
                    default:
                        break;
                }
            }
        }
    }

    for (Chimera::Triangle t : this->trisList) {
        vIndex.push_back(t.p[0]);
        vIndex.push_back(t.p[1]);
        vIndex.push_back(t.p[2]);
    }
}