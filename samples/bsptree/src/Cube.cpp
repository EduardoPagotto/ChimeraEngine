#include "Cube.hpp"

// Cube::Cube(const glm::ivec3& pos, const glm::ivec3& size, const float& sizeBlock) {
Cube::Cube(const char& caracter, const glm::ivec3& pos, const glm::vec3& min, const glm::vec3& max) {

    pNorth = nullptr;
    pEast = nullptr;
    pSouth = nullptr;
    pWest = nullptr;
    pUp = nullptr;
    pBottom = nullptr;

    if (caracter == 0x20)
        this->space = SPACE::EMPTY;
    else
        this->space = (SPACE)(caracter - 0x30);

    this->position = pos;
    this->min = min;
    this->max = max;

    this->p[0] = glm::vec3(min.x, min.y, min.z);
    this->p[1] = glm::vec3(max.x, min.y, min.z);
    this->p[2] = glm::vec3(max.x, max.y, min.z);
    this->p[3] = glm::vec3(min.x, max.y, min.z);
    this->p[4] = glm::vec3(min.x, min.y, max.z);
    this->p[5] = glm::vec3(max.x, min.y, max.z);
    this->p[6] = glm::vec3(max.x, max.y, max.z);
    this->p[7] = glm::vec3(min.x, max.y, max.z);

    this->t[0] = glm::vec2(0, 0);
    this->t[1] = glm::vec2(1, 0);
    this->t[2] = glm::vec2(1, 1);
    this->t[3] = glm::vec2(0, 1);

    this->tVertIndex.push_back(glm::uvec3(0, 1, 2)); // f0 N
    this->tVertIndex.push_back(glm::uvec3(2, 3, 0)); // f1 N
    this->tVertIndex.push_back(glm::uvec3(1, 5, 6)); // f2 E
    this->tVertIndex.push_back(glm::uvec3(6, 2, 1)); // f3 E
    this->tVertIndex.push_back(glm::uvec3(4, 5, 6)); // f4 S
    this->tVertIndex.push_back(glm::uvec3(6, 7, 4)); // f5 S
    this->tVertIndex.push_back(glm::uvec3(0, 4, 7)); // f6 W
    this->tVertIndex.push_back(glm::uvec3(7, 3, 0)); // f7 W
    this->tVertIndex.push_back(glm::uvec3(3, 2, 6)); // f8 UP
    this->tVertIndex.push_back(glm::uvec3(6, 7, 3)); // f9 UP    / C Q0(SW)
    this->tVertIndex.push_back(glm::uvec3(0, 1, 5)); // f10 DOWN /  F Q2(NE)
    this->tVertIndex.push_back(glm::uvec3(5, 4, 0)); // f11 DOWN /  F Q0(SW)

    this->tVertIndex.push_back(glm::uvec3(0, 5, 6)); // f12 DIA1
    this->tVertIndex.push_back(glm::uvec3(6, 3, 0)); // f13 DIA1
    this->tVertIndex.push_back(glm::uvec3(4, 1, 2)); // f14 DIA2
    this->tVertIndex.push_back(glm::uvec3(2, 7, 4)); // f15 DIA2

    this->tVertIndex.push_back(glm::uvec3(0, 1, 6)); // f16 RP NS
    this->tVertIndex.push_back(glm::uvec3(6, 7, 0)); // f17 RP NS
    this->tVertIndex.push_back(glm::uvec3(0, 2, 6)); // f18 RP EW
    this->tVertIndex.push_back(glm::uvec3(6, 4, 0)); // f19 RP EW
    this->tVertIndex.push_back(glm::uvec3(5, 4, 3)); // f20 RP SN
    this->tVertIndex.push_back(glm::uvec3(3, 2, 5)); // f21 RP SN

    this->tVertIndex.push_back(glm::uvec3(1, 5, 7)); // f22 RP WE
    this->tVertIndex.push_back(glm::uvec3(7, 3, 1)); // f23 RP WE

    this->tVertIndex.push_back(glm::uvec3(4, 0, 1)); // f24 F Q1(NW)
    this->tVertIndex.push_back(glm::uvec3(1, 5, 4)); // f25 F Q3(SE)
    this->tVertIndex.push_back(glm::uvec3(7, 3, 2)); // F26 C Q1(NW)
    this->tVertIndex.push_back(glm::uvec3(2, 6, 7)); // F27 C Q3(NW)

    this->tVertIndex.push_back(glm::uvec3(1, 5, 2)); // f28 down ramp ns E
    this->tVertIndex.push_back(glm::uvec3(0, 4, 3)); // f29 down ramp ns W
    this->tVertIndex.push_back(glm::uvec3(4, 7, 3)); // f30 up ramp ns W
    this->tVertIndex.push_back(glm::uvec3(5, 6, 2)); // f31 up ramp ns W

    //---
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // T0 Q0 /   N // FIXME: Trocar estrutura por indice de rotacao de textura!!!!
    this->tTexIndex.push_back(glm::uvec3(2, 3, 0)); // T1 Q2 /
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // T2 !Q0 /
    this->tTexIndex.push_back(glm::uvec3(3, 2, 1)); // T3 !Q2 /
    this->tTexIndex.push_back(glm::uvec3(0, 3, 2)); // T4 F Q1(NW) /
    this->tTexIndex.push_back(glm::uvec3(2, 1, 0)); // T5 F Q3(SE) /
    this->tTexIndex.push_back(glm::uvec3(3, 0, 1)); // T6 c Q1(NW)
    this->tTexIndex.push_back(glm::uvec3(1, 2, 3)); // T7 c

    this->tTexIndex.push_back(glm::uvec3(0, 1, 3)); // T8 f29
    this->tTexIndex.push_back(glm::uvec3(1, 0, 2)); // T9 f29
    this->tTexIndex.push_back(glm::uvec3(0, 3, 2)); // T10 f 30
}

Cube::~Cube() {}

void Cube::setNeighbor(DEEP deep, CARDINAL card, Cube* pCube) {

    switch (deep) {
        case DEEP::UP: {
            if (card == CARDINAL::NONE)
                this->pUp = pCube;
        } break;

        case DEEP::BOTTOM:
            this->pBottom = pCube;
            break;

        case DEEP::MIDDLE: {

            switch (card) {
                case CARDINAL::NORTH:
                    this->pNorth = pCube;
                    break;

                case CARDINAL::EAST:
                    this->pEast = pCube;
                    break;

                case CARDINAL::SOUTH:
                    this->pSouth = pCube;
                    break;

                case CARDINAL::WEST:
                    this->pWest = pCube;
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
}

void Cube::addFace(bool clockwise, int numFace, int numTex, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    glm::uvec3 tri = this->tVertIndex[numFace];
    glm::uvec3 tex = this->tTexIndex[numTex];

    glm::vec3 va = p[tri.x]; // PA
    glm::vec3 vb = p[tri.y]; // PB
    glm::vec3 vc = p[tri.z]; // PC

    glm::vec2 ta = t[tex.x]; // TA
    glm::vec2 tb = t[tex.y]; // TB
    glm::vec2 tc = t[tex.z]; // TC

    uint32_t ia, ib, ic;

    if (!clockwise) {
        ia = tl.size() * 3;
        ib = ia + 1;
        ic = ib + 1;
    } else {
        ic = tl.size() * 3;
        ib = ic + 1;
        ia = ib + 1;
    }

    vl.push_back({va, glm::vec3(0.0f), ta});
    vl.push_back({vb, glm::vec3(0.0f), tb});
    vl.push_back({vc, glm::vec3(0.0f), tc});

    Chimera::Triangle t1 = Chimera::Triangle(ia, ib, ic, glm::vec3(0.0f));
    t1.calcNormal(vl);
    tl.push_back(t1);
}

CARDINAL Cube::emptyQuadrantDiag(DEEP deep, bool invert) {

    Cube* pVal = nullptr;

    switch (deep) {
        case DEEP::UP:
            pVal = this->pUp;
            break;
        case DEEP::MIDDLE:
            pVal = this;
            break;
        case DEEP::BOTTOM:
            pVal = this->pBottom;
            break;
    }

    if (pVal != nullptr) {

        bool isN = (pVal->pNorth != nullptr) ? pVal->pNorth->emptySpace() : false;
        bool isE = (pVal->pEast != nullptr) ? pVal->pEast->emptySpace() : false;
        bool isS = (pVal->pSouth != nullptr) ? pVal->pSouth->emptySpace() : false;
        bool isW = (pVal->pWest != nullptr) ? pVal->pWest->emptySpace() : false;

        if (isN && isE)
            return (!invert) ? CARDINAL::NORTH_EAST : CARDINAL::SOUTH_WEST;

        if (isS && isE)
            return (!invert) ? CARDINAL::SOUTH_EAST : CARDINAL::NORTH_WEST;

        if (isS && isW)
            return (!invert) ? CARDINAL::SOUTH_WEST : CARDINAL::NORTH_EAST;

        if (isN && isW)
            return (!invert) ? CARDINAL::NORTH_WEST : CARDINAL::SOUTH_EAST;
    }

    return CARDINAL::NONE;
}

bool Cube::hasNeighbor(DEEP deep, CARDINAL card, SPACE space) {

    Cube* pVal = nullptr;

    switch (deep) {
        case DEEP::UP:
            pVal = this->pUp;
            break;
        case DEEP::MIDDLE:
            pVal = this;
            break;
        case DEEP::BOTTOM:
            pVal = this->pBottom;
            break;
    }

    if (pVal != nullptr) {

        bool vN = (pVal->pNorth != nullptr) ? (pVal->pNorth->getSpace() == space) : false;
        bool vE = (pVal->pEast != nullptr) ? (pVal->pEast->getSpace() == space) : false;
        bool vS = (pVal->pSouth != nullptr) ? (pVal->pSouth->getSpace() == space) : false;
        bool vW = (pVal->pWest != nullptr) ? (pVal->pWest->getSpace() == space) : false;
        bool cb = (pVal->getSpace() == space);

        switch (card) {
            case CARDINAL::NORTH:
                return vN;
            case CARDINAL::NORTH_EAST:
                return (vN && vE);
            case CARDINAL::EAST:
                return vE;
            case CARDINAL::SOUTH_EAST:
                return (vS && vE);
            case CARDINAL::SOUTH:
                return vS;
            case CARDINAL::SOUTH_WEST:
                return (vS && vW);
            case CARDINAL::WEST:
                return vW;
            case CARDINAL::NORTH_WEST:
                return (vN && vW);
            case CARDINAL::NONE:
                return cb;
            default:
                break;
        }
    }

    return false;
}

void Cube::newWall(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    if ((this->pNorth != nullptr) && (this->pNorth->getSpace() == SPACE::WALL)) {
        this->addFace(false, 0, 0, vl, tl);
        this->addFace(false, 1, 1, vl, tl);
    }

    if ((this->pEast != nullptr) && (this->pEast->getSpace() == SPACE::WALL)) {
        this->addFace(false, 2, 0, vl, tl);
        this->addFace(false, 3, 1, vl, tl);
    }

    if ((this->pSouth != nullptr) && (this->pSouth->getSpace() == SPACE::WALL)) {
        this->addFace(true, 4, 2, vl, tl);
        this->addFace(true, 5, 3, vl, tl);
    }

    if ((this->pWest != nullptr) && (this->pWest->getSpace() == SPACE::WALL)) {
        this->addFace(true, 6, 2, vl, tl);
        this->addFace(true, 7, 3, vl, tl);
    }
}

void Cube::newRamp(bool isFloor, CARDINAL card, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {
    if (isFloor) {
        switch (card) {
            case CARDINAL::NORTH:
                this->addFace(true, 20, 2, vl, tl); // ok
                this->addFace(true, 21, 3, vl, tl); // ok
                break;
            case CARDINAL::EAST:
                this->addFace(true, 18, 4, vl, tl); // ok
                this->addFace(true, 19, 5, vl, tl); // ok
                break;
            case CARDINAL::SOUTH:
                this->addFace(true, 16, 2, vl, tl); // ok
                this->addFace(true, 17, 3, vl, tl); // ok
                break;
            case CARDINAL::WEST:
                this->addFace(true, 22, 2, vl, tl); // ok
                this->addFace(true, 23, 3, vl, tl); // ok
                break;
            default:
                break;
        }
    } else {
        switch (card) {
            case CARDINAL::NORTH:
                this->addFace(false, 16, 0, vl, tl); // ok
                this->addFace(false, 17, 1, vl, tl); // ok
                break;
            case CARDINAL::EAST:
                this->addFace(false, 22, 0, vl, tl); // ok
                this->addFace(false, 23, 1, vl, tl); // ok
                break;
            case CARDINAL::SOUTH:
                this->addFace(false, 20, 0, vl, tl); // ok
                this->addFace(false, 21, 1, vl, tl); // ok
                break;
            case CARDINAL::WEST:
                this->addFace(false, 18, 7, vl, tl); // ok
                this->addFace(false, 19, 6, vl, tl); // ok
                break;
            default:
                break;
        }
    }
}

void Cube::newDiag(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    // get side
    CARDINAL card = this->emptyQuadrantDiag(DEEP::MIDDLE, false);

    switch (card) {
        case CARDINAL::SOUTH_WEST:
            // ne (diag. sup. dir.)
            this->addFace(false, 12, 0, vl, tl);
            this->addFace(false, 13, 1, vl, tl);
            break;
        case CARDINAL::NORTH_WEST:
            // se (diag. inf. dir.)
            this->addFace(true, 14, 2, vl, tl);
            this->addFace(true, 15, 3, vl, tl);
            break;
        case CARDINAL::NORTH_EAST:
            // sw (diag. inf. esq.)
            this->addFace(true, 12, 2, vl, tl);
            this->addFace(true, 13, 3, vl, tl);
            break;
        case CARDINAL::SOUTH_EAST:
            // nw (diag. sup. esq.)
            this->addFace(false, 14, 0, vl, tl);
            this->addFace(false, 15, 1, vl, tl);
            break;
        default:
            break;
    }

    // floor of wall diag
    if (this->hasNeighbor(DEEP::MIDDLE, card, SPACE::FLOOR) == true)
        newFlatFloorCeeling(true, card, vl, tl);

    // ceeling of wall diag
    if (this->hasNeighbor(DEEP::MIDDLE, card, SPACE::CEILING) == true)
        newFlatFloorCeeling(false, card, vl, tl);

    if (this->hasNeighbor(DEEP::MIDDLE, card, SPACE::FC) == true) {
        newFlatFloorCeeling(true, card, vl, tl);
        newFlatFloorCeeling(false, card, vl, tl);
    }
}

void Cube::newFloor(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    CARDINAL card = CARDINAL::NONE;
    if ((this->pBottom != nullptr) && (this->pBottom->getSpace() == SPACE::DIAG))
        card = this->emptyQuadrantDiag(DEEP::BOTTOM, true);

    this->newFlatFloorCeeling(true, card, vl, tl);
}

void Cube::newCeeling(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    CARDINAL card = CARDINAL::NONE;
    if ((this->pUp != nullptr) && (this->pUp->getSpace() == SPACE::DIAG))
        card = this->emptyQuadrantDiag(DEEP::UP, true);

    this->newFlatFloorCeeling(false, card, vl, tl);
}

void Cube::newFlatFloorCeeling(bool isFloor, CARDINAL card, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    if (isFloor) {
        switch (card) {
            case CARDINAL::SOUTH_WEST:
                this->addFace(true, 11, 2, vl, tl);
                break;
            case CARDINAL::NORTH_WEST:
                this->addFace(true, 24, 4, vl, tl);
                break;
            case CARDINAL::NORTH_EAST:
                this->addFace(true, 10, 3, vl, tl);
                break;
            case CARDINAL::SOUTH_EAST:
                this->addFace(true, 25, 5, vl, tl);
                break;
            default:
                this->addFace(true, 10, 3, vl, tl);
                this->addFace(true, 11, 2, vl, tl);
                break;
        }
    } else {
        switch (card) {
            case CARDINAL::SOUTH_WEST:
                this->addFace(false, 9, 1, vl, tl);
                break;
            case CARDINAL::NORTH_WEST:
                this->addFace(false, 26, 6, vl, tl);
                break;
            case CARDINAL::NORTH_EAST:
                this->addFace(false, 8, 0, vl, tl);
                break;
            case CARDINAL::SOUTH_EAST:
                this->addFace(false, 27, 7, vl, tl);
                break;
            default:
                this->addFace(false, 8, 0, vl, tl);
                this->addFace(false, 9, 1, vl, tl);
                break;
        }
    }
}

void Cube::newRampNS(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    if ((pNorth != nullptr) && (pNorth->emptySpace())) {

        SPACE vazio = pNorth->getSpace();
        if (vazio == SPACE::FLOOR)
            this->newRamp(true, CARDINAL::SOUTH, vl, tl);
        else if (vazio == SPACE::CEILING) {
            this->newRamp(false, CARDINAL::SOUTH, vl, tl);
            // down west
            if ((pWest != nullptr) && (pWest->emptySpace())) {
                this->addFace(false, 30, 7, vl, tl);
            }

            // down east
            if ((pEast != nullptr) && (pEast->emptySpace())) {
                this->addFace(true, 31, 10, vl, tl);
            }

            // up West
            if ((pWest != nullptr) && (pWest->getSpace() == SPACE::WALL)) {
                this->addFace(false, 29, 9, vl, tl); // ---
            }

            // up east
            if ((pEast != nullptr) && (pEast->getSpace() == SPACE::WALL)) {
                this->addFace(true, 28, 8, vl, tl);
            }
        }

    } else if ((pSouth != nullptr) && (pSouth->emptySpace())) {

        SPACE vazio = pSouth->getSpace();

        if (vazio == SPACE::FLOOR) {
            this->newRamp(true, CARDINAL::NORTH, vl, tl);

            // down west
            if ((pWest != nullptr) && (pWest->emptySpace())) {
                this->addFace(false, 29, 8, vl, tl);
            }

            // down east
            if ((pEast != nullptr) && (pEast->emptySpace())) {
                this->addFace(true, 28, 9, vl, tl);
            }

            // up West
            if ((pWest != nullptr) && (pWest->getSpace() == SPACE::WALL)) {
                this->addFace(false, 30, 10, vl, tl); // ---
            }

            // up east
            if ((pEast != nullptr) && (pEast->getSpace() == SPACE::WALL)) {
                this->addFace(true, 31, 7, vl, tl);
            }
        } else if (vazio == SPACE::CEILING) {
            // this->newRamp(false, CARDINAL::NORTH, vl, tl);
        }
    }

    // bool isN = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos));
    // bool isE = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos));
    // bool isS = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos));
    // bool isW = this->empty(this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos));

    // if (isN && (!isS)) {

    //     SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos);
    //     if (vazio == SPACE::FLOOR)
    //         pCube->newRamp(true, CARDINAL::SOUTH, this->vertexData, this->trisList);
    //     else if (vazio == SPACE::CEILING)
    //         pCube->newRamp(false, CARDINAL::SOUTH, this->vertexData, this->trisList);

    // } else if (isE && (!isW)) {

    //     SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos);
    //     if (vazio == SPACE::FLOOR)
    //         pCube->newRamp(true, CARDINAL::WEST, this->vertexData, this->trisList);
    //     else if (vazio == SPACE::CEILING)
    //         pCube->newRamp(false, CARDINAL::WEST, this->vertexData, this->trisList);

    // } else if ((!isN) && isS) {

    //     SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos);
    //     if (vazio == SPACE::FLOOR)
    //         pCube->newRamp(true, CARDINAL::NORTH, this->vertexData, this->trisList);
    //     else if (vazio == SPACE::CEILING)
    //         pCube->newRamp(false, CARDINAL::NORTH, this->vertexData, this->trisList);

    // } else if ((!isE) && isW) {

    //     SPACE vazio = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos);
    //     if (vazio == SPACE::FLOOR)
    //         pCube->newRamp(true, CARDINAL::EAST, this->vertexData, this->trisList);
    //     else if (vazio == SPACE::CEILING)
    //         pCube->newRamp(false, CARDINAL::EAST, this->vertexData, this->trisList);
    // }
}