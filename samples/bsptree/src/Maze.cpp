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

void Maze::newWall(const glm::ivec3& pos) {
    Cube* pCube = this->vpCube[this->getIndexArrayPos(pos)];

    if (SPACE::WALL == this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos)) {
        pCube->addFace(false, 0, 0, vertexData, this->trisList);
        pCube->addFace(false, 1, 1, vertexData, this->trisList);
    }

    if (SPACE::WALL == this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos)) {
        pCube->addFace(false, 2, 0, vertexData, this->trisList);
        pCube->addFace(false, 3, 1, vertexData, this->trisList);
    }

    if (SPACE::WALL == this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos)) {
        pCube->addFace(true, 4, 2, vertexData, this->trisList);
        pCube->addFace(true, 5, 3, vertexData, this->trisList);
    }

    if (SPACE::WALL == this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos)) {
        pCube->addFace(true, 6, 2, vertexData, this->trisList);
        pCube->addFace(true, 7, 3, vertexData, this->trisList);
    }
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

CARDINAL Maze::emptyQuadrantDiag(bool invert, const glm::ivec3& pos) {

    bool isN = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos));
    bool isE = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos));
    bool isS = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos));
    bool isW = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos));

    if (isN && isE)
        return (!invert) ? CARDINAL::NORTH_EAST : CARDINAL::SOUTH_WEST;

    if (isS && isE)
        return (!invert) ? CARDINAL::SOUTH_EAST : CARDINAL::NORTH_WEST;

    if (isS && isW)
        return (!invert) ? CARDINAL::SOUTH_WEST : CARDINAL::NORTH_EAST;

    if (isN && isW)
        return (!invert) ? CARDINAL::NORTH_WEST : CARDINAL::SOUTH_EAST;

    return CARDINAL::NONE;
}

void Maze::newRamp(bool isFloor, CARDINAL card, const glm::ivec3& pos) {

    Cube* pCube = this->vpCube[this->getIndexArrayPos(pos)];
    if (card == CARDINAL::NORTH) { // normal in SW

        if (isFloor) {
            pCube->addFace(true, 20, 2, vertexData, this->trisList); // ok
            pCube->addFace(true, 21, 3, vertexData, this->trisList); // ok
        } else {
            pCube->addFace(false, 16, 0, vertexData, this->trisList); // ok
            pCube->addFace(false, 17, 1, vertexData, this->trisList); // ok
        }

    } else if (card == CARDINAL::EAST) { // normal in NW

        if (isFloor) {
            pCube->addFace(true, 18, 4, vertexData, this->trisList); // ok
            pCube->addFace(true, 19, 5, vertexData, this->trisList); // ok
        } else {
            pCube->addFace(false, 22, 0, vertexData, this->trisList); // ok
            pCube->addFace(false, 23, 1, vertexData, this->trisList); // ok
        }

    } else if (card == CARDINAL::SOUTH) { // normal in NE

        if (isFloor) {
            pCube->addFace(true, 16, 2, vertexData, this->trisList); // ok
            pCube->addFace(true, 17, 3, vertexData, this->trisList); // ok
        } else {
            pCube->addFace(false, 20, 0, vertexData, this->trisList); // ok
            pCube->addFace(false, 21, 1, vertexData, this->trisList); // ok
        }

    } else if (card == CARDINAL::WEST) { // normal in SE

        if (isFloor) {
            pCube->addFace(true, 22, 2, vertexData, this->trisList); // ok
            pCube->addFace(true, 23, 3, vertexData, this->trisList); // ok
        } else {
            pCube->addFace(false, 18, 7, vertexData, this->trisList); // ok
            pCube->addFace(false, 19, 6, vertexData, this->trisList); // ok
        }
    }
}

void Maze::newDiag(CARDINAL card, const glm::ivec3& pos) {

    bool clockwise = true;
    Cube* pCube = this->vpCube[this->getIndexArrayPos(pos)];
    if (card == CARDINAL::SOUTH_WEST) { // normal in SW
        // ne (diag. sup. dir.)
        pCube->addFace(false, 12, 0, vertexData, this->trisList);
        pCube->addFace(false, 13, 1, vertexData, this->trisList);
    } else if (card == CARDINAL::NORTH_WEST) { // normal in NW
        // se (diag. inf. dir.)
        pCube->addFace(true, 14, 2, vertexData, this->trisList);
        pCube->addFace(true, 15, 3, vertexData, this->trisList);
    } else if (card == CARDINAL::NORTH_EAST) { // normal in NE
        // sw (diag. inf. esq.)
        pCube->addFace(true, 12, 2, vertexData, this->trisList);
        pCube->addFace(true, 13, 3, vertexData, this->trisList);
    } else if (card == CARDINAL::SOUTH_EAST) { // normal in SE
        // nw (diag. sup. esq.)
        pCube->addFace(false, 14, 0, vertexData, this->trisList);
        pCube->addFace(false, 15, 1, vertexData, this->trisList);
    }
}

void Maze::newFlatFloorCeeling(bool isFloor, CARDINAL card, const glm::ivec3& pos) {

    Cube* pCube = this->vpCube[this->getIndexArrayPos(pos)];
    if (card == CARDINAL::SOUTH_WEST) { // quad == 0) {
        if (isFloor) {                  // floor
            pCube->addFace(true, 11, 2, vertexData, this->trisList);
        } else { // celling
            pCube->addFace(false, 9, 1, vertexData, this->trisList);
        }
    } else if (card == CARDINAL::NORTH_WEST) { // uad == 1) {
        if (isFloor) {
            pCube->addFace(true, 24, 4, vertexData, this->trisList);
        } else {
            pCube->addFace(false, 26, 6, vertexData, this->trisList);
        }
    } else if (card == CARDINAL::NORTH_EAST) { // quad == 2) {
        if (isFloor) {
            pCube->addFace(true, 10, 3, vertexData, this->trisList);
        } else {
            pCube->addFace(false, 8, 0, vertexData, this->trisList);
        }
    } else if (card == CARDINAL::SOUTH_EAST) { // quad == 3) {
        if (isFloor) {
            pCube->addFace(true, 25, 5, vertexData, this->trisList);
        } else {
            pCube->addFace(false, 27, 7, vertexData, this->trisList);
        }
    } else { // CARDINAL::NONE
        if (isFloor) {
            pCube->addFace(true, 10, 3, vertexData, this->trisList);
            pCube->addFace(true, 11, 2, vertexData, this->trisList);
        } else {
            pCube->addFace(false, 8, 0, vertexData, this->trisList);
            pCube->addFace(false, 9, 1, vertexData, this->trisList);
        }
    }
}

// // lembrar d = y;  w = x;  h = z;
void Maze::createMap() {
    glm::ivec3 pos;
    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {

                SPACE val = this->getCardinal(pos);
                switch (val) {
                    case SPACE::EMPTY:
                        this->newWall(pos);
                        break;
                    case SPACE::FLOOR:
                        this->newWall(pos);
                        if (SPACE::DIAG == this->getCardinalNeighbor(DEEP::BOTTOM, CARDINAL::NONE, glm::ivec3(1), pos)) {
                            CARDINAL normDiag =
                                this->emptyQuadrantDiag(true, this->getCardinalPos(DEEP::BOTTOM, CARDINAL::NONE, glm::ivec3(1), pos));
                            this->newFlatFloorCeeling(true, normDiag, pos);
                        } else {
                            this->newFlatFloorCeeling(true, CARDINAL::NONE, pos);
                        }
                        break;
                    case SPACE::CEILING:
                        this->newWall(pos);

                        if (SPACE::DIAG == this->getCardinalNeighbor(DEEP::UP, CARDINAL::NONE, glm::ivec3(1), pos)) {
                            CARDINAL normDiag =
                                this->emptyQuadrantDiag(true, this->getCardinalPos(DEEP::UP, CARDINAL::NONE, glm::ivec3(1), pos));
                            this->newFlatFloorCeeling(false, normDiag, pos);
                        } else {
                            this->newFlatFloorCeeling(false, CARDINAL::NONE, pos);
                        }

                        break;
                    case SPACE::FC:
                        this->newWall(pos);
                        this->newFlatFloorCeeling(true, CARDINAL::NONE, pos);
                        this->newFlatFloorCeeling(false, CARDINAL::NONE, pos);
                        break;
                    case SPACE::DIAG: {
                        CARDINAL normDiag = this->emptyQuadrantDiag(false, pos);
                        this->newDiag(normDiag, pos);

                        if (this->hasNeighbor(normDiag, SPACE::FLOOR, pos) == true)
                            this->newFlatFloorCeeling(true, normDiag, pos);

                        if (this->hasNeighbor(normDiag, SPACE::CEILING, pos) == true)
                            this->newFlatFloorCeeling(false, normDiag, pos);

                        if (this->hasNeighbor(normDiag, SPACE::FC, pos) == true) {
                            this->newFlatFloorCeeling(true, normDiag, pos);
                            this->newFlatFloorCeeling(false, normDiag, pos);
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
                                this->newRamp(true, CARDINAL::SOUTH, pos);
                            else if (vazio == SPACE::CEILING)
                                this->newRamp(false, CARDINAL::SOUTH, pos);

                        } else if (isE && (!isW)) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                this->newRamp(true, CARDINAL::WEST, pos);
                            else if (vazio == SPACE::CEILING)
                                this->newRamp(false, CARDINAL::WEST, pos);

                        } else if ((!isN) && isS) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                this->newRamp(true, CARDINAL::NORTH, pos);
                            else if (vazio == SPACE::CEILING)
                                this->newRamp(false, CARDINAL::NORTH, pos);

                        } else if ((!isE) && isW) {

                            SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos);
                            if (vazio == SPACE::FLOOR)
                                this->newRamp(true, CARDINAL::EAST, pos);
                            else if (vazio == SPACE::CEILING)
                                this->newRamp(false, CARDINAL::EAST, pos);
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