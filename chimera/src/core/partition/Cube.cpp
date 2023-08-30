#include "chimera/core/partition/Cube.hpp"

namespace Chimera {

static std::vector<glm::uvec3> tVertIndex;
static std::vector<glm::uvec3> tTexIndex;
static std::vector<glm::vec2> tTexSeq;

void initCubeBase() {
    tVertIndex.push_back(glm::uvec3(0, 1, 3)); // f00 N0
    tVertIndex.push_back(glm::uvec3(1, 3, 2)); // f01 N1
    tVertIndex.push_back(glm::uvec3(3, 2, 0)); // f02 N2
    tVertIndex.push_back(glm::uvec3(2, 0, 1)); // f03 N3

    tVertIndex.push_back(glm::uvec3(1, 5, 7)); // f04 E0
    tVertIndex.push_back(glm::uvec3(5, 7, 3)); // f05 E1 RPNEU
    tVertIndex.push_back(glm::uvec3(7, 3, 1)); // f06 E2
    tVertIndex.push_back(glm::uvec3(3, 1, 5)); // f07 E3 RPNED

    tVertIndex.push_back(glm::uvec3(5, 4, 6)); // f08 S0
    tVertIndex.push_back(glm::uvec3(4, 6, 7)); // f09 S1
    tVertIndex.push_back(glm::uvec3(6, 7, 5)); // f10 S2
    tVertIndex.push_back(glm::uvec3(7, 5, 4)); // f11 S3

    tVertIndex.push_back(glm::uvec3(4, 0, 2)); // f12 W0 RPNWD
    tVertIndex.push_back(glm::uvec3(0, 2, 6)); // f13 W1
    tVertIndex.push_back(glm::uvec3(2, 6, 4)); // f14 W2 RPWU
    tVertIndex.push_back(glm::uvec3(6, 4, 0)); // f15 W3

    tVertIndex.push_back(glm::uvec3(7, 6, 2)); // f16 U0
    tVertIndex.push_back(glm::uvec3(6, 2, 3)); // f17 U1
    tVertIndex.push_back(glm::uvec3(2, 3, 7)); // f18 U2
    tVertIndex.push_back(glm::uvec3(3, 7, 6)); // f19 U3

    tVertIndex.push_back(glm::uvec3(4, 5, 1)); // f20 D0
    tVertIndex.push_back(glm::uvec3(5, 1, 0)); // f21 D1
    tVertIndex.push_back(glm::uvec3(1, 0, 4)); // f22 D2
    tVertIndex.push_back(glm::uvec3(0, 4, 5)); // f23 D3
                                               //--
    tVertIndex.push_back(glm::uvec3(0, 5, 7)); // f24 DIA1
    tVertIndex.push_back(glm::uvec3(7, 2, 0)); // f25 DIA1
    tVertIndex.push_back(glm::uvec3(4, 1, 3)); // f26 DIA2
    tVertIndex.push_back(glm::uvec3(3, 6, 4)); // f27 DIA2

    tVertIndex.push_back(glm::uvec3(4, 5, 3)); // f28 RP NS
    tVertIndex.push_back(glm::uvec3(3, 2, 4)); // f29 RP NS
    tVertIndex.push_back(glm::uvec3(0, 4, 7)); // f30 RP EW
    tVertIndex.push_back(glm::uvec3(7, 3, 0)); // f31 RP EW

    tVertIndex.push_back(glm::uvec3(1, 0, 6)); // f32 RP SN
    tVertIndex.push_back(glm::uvec3(6, 7, 1)); // f33 RP SN
    tVertIndex.push_back(glm::uvec3(5, 1, 2)); // f34 RP WE
    tVertIndex.push_back(glm::uvec3(2, 6, 5)); // f35 RP WE

    //---
    tTexIndex.push_back(glm::uvec3(0, 2, 3)); // T0 Q0
    tTexIndex.push_back(glm::uvec3(3, 1, 0)); // T1 Q2
    tTexIndex.push_back(glm::uvec3(2, 0, 1)); // T2 !Q0
    tTexIndex.push_back(glm::uvec3(1, 3, 2)); // T3 !Q2

    tTexIndex.push_back(glm::uvec3(1, 0, 2)); // T4 F Q1(NW)
    tTexIndex.push_back(glm::uvec3(2, 3, 1)); // T5 F Q3(SE)
    tTexIndex.push_back(glm::uvec3(2, 0, 1)); // T6 c Q1(NW)
    tTexIndex.push_back(glm::uvec3(1, 3, 2)); // T7 c

    tTexIndex.push_back(glm::uvec3(3, 2, 0)); // T8 c
    tTexIndex.push_back(glm::uvec3(0, 1, 3)); // T9 c

    // ---
    tTexSeq.push_back(glm::vec2(0, 0)); // 0
    tTexSeq.push_back(glm::vec2(0, 1)); // 1
    tTexSeq.push_back(glm::vec2(1, 0)); // 2
    tTexSeq.push_back(glm::vec2(1, 1)); // 3
}

void cleanupCubeBase() {
    tVertIndex.clear();
    tTexIndex.clear();
    tTexSeq.clear();
}

glm::ivec3 getCardinalPos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos) {
    glm::ivec3 val = pos;
    switch (deep) {
        case DEEP::UP:
            val.y = pos.y + dist.y;
            break;
        case DEEP::MIDDLE:
            val.y = pos.y;
            break;
        case DEEP::DOWN:
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

Cube* getCubeNeighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos, const glm::ivec3& size, std::vector<Cube*>& vpCube) {

    glm::ivec3 val = getCardinalPos(deep, card, glm::ivec3(1), pos);
    // get Valid position
    if ((val.z >= 0) && (val.z < size.z) && (val.x >= 0) && (val.x < size.x) && (val.y >= 0) && (val.y < size.y))
        return vpCube[getIndexArrayPos(val, size)];

    return nullptr;
}

glm::vec3 minimal(const float& sizeBlock, const glm::vec3 halfBlock, const glm::ivec3& pos) {
    float x_min = (pos.x * sizeBlock) - halfBlock.x; // (width++)
    float y_min = (pos.y * sizeBlock) - halfBlock.y; // Altura Minima andar
    float z_min = (pos.z * sizeBlock) - halfBlock.z; // (height--)
    return glm::vec3(x_min, y_min, z_min);
}

uint32_t getIndexArrayPos(const glm::ivec3& pos, const glm::ivec3& size) { return pos.x + (pos.z * size.x) + (pos.y * size.x * size.z); }

Cube::Cube(const char& caracter, const glm::vec3& min, const glm::vec3& max)
    : AABB(min, max), pNorth(nullptr), pEast(nullptr), pSouth(nullptr), pWest(nullptr), pUp(nullptr), pDown(nullptr) {

    space = (caracter == 0x20) ? SPACE::EMPTY : (SPACE)(caracter - 0x30);
}

void linkCubes(const glm::ivec3& size, std::vector<Cube*>& vpCube) {
    glm::ivec3 pos(0);
    for (pos.y = 0; pos.y < size.y; pos.y++) {
        for (pos.z = 0; pos.z < size.z; pos.z++) {
            for (pos.x = 0; pos.x < size.x; pos.x++) {
                Cube* pCube = vpCube[getIndexArrayPos(pos, size)];

                Cube* pBefore = getCubeNeighbor(DEEP::DOWN, CARDINAL::NONE, pos, size, vpCube);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::DOWN, CARDINAL::NONE, pBefore);

                pBefore = getCubeNeighbor(DEEP::UP, CARDINAL::NONE, pos, size, vpCube);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::UP, CARDINAL::NONE, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, pos, size, vpCube);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::EAST, pos, size, vpCube);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::EAST, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, pos, size, vpCube);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, pBefore);

                pBefore = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::WEST, pos, size, vpCube);
                if (pBefore != nullptr)
                    pCube->setNeighbor(DEEP::MIDDLE, CARDINAL::WEST, pBefore);
            }
        }
    }
}

//-----

Cube::~Cube() {}

void Cube::setNeighbor(DEEP deep, CARDINAL card, Cube* pCube) {

    switch (deep) {
        case DEEP::UP: {
            if (card == CARDINAL::NONE)
                this->pUp = pCube;
        } break;

        case DEEP::DOWN:
            this->pDown = pCube;
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

void Cube::addFace(bool clockwise, int numFace, int numTex) {

    glm::uvec3 tri = tVertIndex[numFace];
    glm::uvec3 tex = tTexIndex[numTex];

    glm::vec3 va = vertex[tri.x]; // PA
    glm::vec3 vb = vertex[tri.y]; // PB
    glm::vec3 vc = vertex[tri.z]; // PC

    glm::vec2 ta = tTexSeq[tex.x]; // TA
    glm::vec2 tb = tTexSeq[tex.y]; // TB
    glm::vec2 tc = tTexSeq[tex.z]; // TC

    uint32_t ia, ib, ic;
    if (!clockwise) {
        ia = mesh->iPoint.size() * 3; // tl->size() * 3;
        ib = ia + 1;
        ic = ib + 1;
    } else {
        ic = mesh->iPoint.size() * 3; // tl->size() * 3;
        ib = ic + 1;
        ia = ib + 1;
    }

    const glm::uvec3 face(ia, ib, ic);

    glm::vec3 vn = glm::normalize(glm::cross(vb - va, vc - va)); // CROSS(U,V)

    mesh->point.push_back(va);
    mesh->point.push_back(vb);
    mesh->point.push_back(vc);

    mesh->uv.push_back(ta);
    mesh->uv.push_back(tb);
    mesh->uv.push_back(tc);

    mesh->normal.push_back(vn);
    mesh->normal.push_back(vn);
    mesh->normal.push_back(vn);

    mesh->iPoint.push_back(face);
    mesh->iNormal.push_back(face);
    mesh->iUv.push_back(face);
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
        case DEEP::DOWN:
            pVal = this->pDown;
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
        case DEEP::DOWN:
            pVal = this->pDown;
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

void Cube::newWall() {

    if ((this->pNorth != nullptr) && (this->pNorth->getSpace() == SPACE::SOLID)) {
        this->addFace(false, 0, 0);
        this->addFace(false, 2, 1);
    }

    if ((this->pEast != nullptr) && (this->pEast->getSpace() == SPACE::SOLID)) {
        this->addFace(false, 4, 0);
        this->addFace(false, 6, 1);
    }

    if ((this->pSouth != nullptr) && (this->pSouth->getSpace() == SPACE::SOLID)) {
        this->addFace(false, 8, 0);
        this->addFace(false, 10, 1);
    }

    if ((this->pWest != nullptr) && (this->pWest->getSpace() == SPACE::SOLID)) {
        this->addFace(false, 12, 0);
        this->addFace(false, 14, 1);
    }
}

void Cube::newRamp(bool isFloor, CARDINAL card) {

    bool westWallDown = false;
    bool westWallUp = false;
    bool eastWallDown = false;
    bool eastWallUp = false;

    bool northWallDown = false;
    bool northWallUp = false;
    bool southWallDown = false;
    bool southWallUp = false;

    if (pWest != nullptr) {
        westWallDown = pWest->emptySpace();
        westWallUp = (pWest->getSpace() == SPACE::SOLID);
    }

    if (pEast != nullptr) {
        eastWallDown = pEast->emptySpace();
        eastWallUp = (pEast->getSpace() == SPACE::SOLID);
    }

    if (pNorth != nullptr) {
        northWallDown = pNorth->emptySpace();
        northWallUp = (pNorth->getSpace() == SPACE::SOLID);
    }

    if (pSouth != nullptr) {
        southWallDown = pSouth->emptySpace();
        southWallUp = (pSouth->getSpace() == SPACE::SOLID);
    }

    if (isFloor) {
        switch (card) {
            case CARDINAL::NORTH: { // OK
                this->addFace(false, 28, 0);
                this->addFace(false, 29, 1);
                if (westWallDown)
                    this->addFace(true, 12, 2);
                if (westWallUp)
                    this->addFace(false, 14, 1);
                if (eastWallDown)
                    this->addFace(true, 7, 8);
                if (eastWallUp)
                    this->addFace(false, 5, 5);
            } break;
            case CARDINAL::EAST: {
                this->addFace(false, 30, 0);
                this->addFace(false, 31, 1);

                if (northWallDown) // OK
                    this->addFace(true, 0, 2);
                if (northWallUp)
                    this->addFace(false, 2, 1);
                if (southWallDown)
                    this->addFace(true, 11, 8);
                if (southWallUp)
                    this->addFace(false, 9, 5);

            } break;
            case CARDINAL::SOUTH: { // OK
                this->addFace(false, 32, 0);
                this->addFace(false, 33, 1);
                if (westWallDown)
                    this->addFace(true, 15, 8);
                if (westWallUp)
                    this->addFace(false, 13, 5);
                if (eastWallDown)
                    this->addFace(true, 4, 6);
                if (eastWallUp)
                    this->addFace(false, 6, 1);
            } break;
            case CARDINAL::WEST: { // OK
                this->addFace(false, 34, 0);
                this->addFace(false, 35, 1);

                if (northWallDown)
                    this->addFace(true, 3, 8);
                if (northWallUp)
                    this->addFace(false, 1, 5);
                if (southWallDown)
                    this->addFace(true, 8, 2);
                if (southWallUp)
                    this->addFace(false, 10, 1);

            } break;
            default:
                break;
        }
    } else {
        switch (card) { // OK
            case CARDINAL::NORTH: {
                this->addFace(true, 32, 2);
                this->addFace(true, 33, 3);

                if (westWallDown)
                    this->addFace(true, 13, 9);
                if (westWallUp)
                    this->addFace(false, 15, 4);
                if (eastWallDown)
                    this->addFace(true, 6, 3);
                if (eastWallUp)
                    this->addFace(false, 4, 0);

            } break;
            case CARDINAL::EAST:
                this->addFace(true, 34, 2);
                this->addFace(true, 35, 3);

                if (northWallDown)
                    this->addFace(true, 1, 9);
                if (northWallUp)
                    this->addFace(false, 3, 4);
                if (southWallDown)
                    this->addFace(true, 10, 3);
                if (southWallUp)
                    this->addFace(false, 8, 0);

                break;
            case CARDINAL::SOUTH: {
                this->addFace(true, 28, 2);
                this->addFace(true, 29, 3);

                if (westWallDown)
                    this->addFace(true, 14, 3);
                if (westWallUp)
                    this->addFace(false, 12, 0);
                if (eastWallDown)
                    this->addFace(true, 5, 9);
                if (eastWallUp)
                    this->addFace(false, 7, 4);
            } break;
            case CARDINAL::WEST: {
                this->addFace(true, 30, 2);
                this->addFace(true, 31, 3);

                if (northWallDown)
                    this->addFace(true, 2, 3);
                if (northWallUp)
                    this->addFace(false, 0, 0);
                if (southWallDown)
                    this->addFace(true, 9, 9);
                if (southWallUp)
                    this->addFace(false, 11, 4);

            } break;
            default:
                break;
        }
    }
}

void Cube::newDiag() {

    // get side
    CARDINAL card = this->emptyQuadrantDiag(DEEP::MIDDLE, false);

    switch (card) {
        case CARDINAL::SOUTH_WEST:
            // ne (diag. sup. dir.)
            this->addFace(false, 24, 0);
            this->addFace(false, 25, 1);
            break;
        case CARDINAL::NORTH_WEST:
            // se (diag. inf. dir.)
            this->addFace(true, 26, 2);
            this->addFace(true, 27, 3);
            break;
        case CARDINAL::NORTH_EAST:
            // sw (diag. inf. esq.)
            this->addFace(true, 24, 2);
            this->addFace(true, 25, 3);
            break;
        case CARDINAL::SOUTH_EAST:
            // nw (diag. sup. esq.)
            this->addFace(false, 26, 0);
            this->addFace(false, 27, 1);
            break;
        default:
            break;
    }

    // floor of wall diag
    if (this->hasNeighbor(DEEP::MIDDLE, card, SPACE::FLOOR) == true)
        newFlatFloorCeeling(true, card);

    // ceeling of wall diag
    if (this->hasNeighbor(DEEP::MIDDLE, card, SPACE::CEILING) == true)
        newFlatFloorCeeling(false, card);

    if (this->hasNeighbor(DEEP::MIDDLE, card, SPACE::FC) == true) {
        newFlatFloorCeeling(true, card);
        newFlatFloorCeeling(false, card);
    }
}

void Cube::newFloor() {

    CARDINAL card = CARDINAL::NONE;
    if ((this->pDown != nullptr) && (this->pDown->getSpace() == SPACE::DIAG))
        card = this->emptyQuadrantDiag(DEEP::DOWN, true);

    this->newFlatFloorCeeling(true, card);
}

void Cube::newCeeling() {

    CARDINAL card = CARDINAL::NONE;
    if ((this->pUp != nullptr) && (this->pUp->getSpace() == SPACE::DIAG))
        card = this->emptyQuadrantDiag(DEEP::UP, true);

    this->newFlatFloorCeeling(false, card);
}

void Cube::newFlatFloorCeeling(bool isFloor, CARDINAL card) {

    if (isFloor) {
        switch (card) {
            case CARDINAL::SOUTH_WEST:
                this->addFace(false, 23, 4);
                break;
            case CARDINAL::NORTH_WEST:
                this->addFace(false, 22, 1);
                break;
            case CARDINAL::NORTH_EAST:
                this->addFace(false, 21, 5);
                break;
            case CARDINAL::SOUTH_EAST:
                this->addFace(false, 20, 0);
                break;
            default:
                this->addFace(false, 20, 0);
                this->addFace(false, 22, 1);
                break;
        }
    } else {
        switch (card) {
            case CARDINAL::SOUTH_WEST:
                this->addFace(false, 16, 0);
                break;
            case CARDINAL::NORTH_WEST:
                this->addFace(false, 17, 5);
                break;
            case CARDINAL::NORTH_EAST:
                this->addFace(false, 18, 1);
                break;
            case CARDINAL::SOUTH_EAST:
                this->addFace(false, 19, 4);
                break;
            default:
                this->addFace(false, 16, 0);
                this->addFace(false, 18, 1);
                break;
        }
    }
}

void Cube::newRampNSEW(SPACE space) {

    if (space == SPACE::RAMP_FNS) {
        if ((pNorth != nullptr) && (pNorth->emptySpace())) {

            bool isFloor = (pNorth->getSpace() == SPACE::FLOOR);
            this->newRamp(isFloor, CARDINAL::SOUTH);

        } else if ((pSouth != nullptr) && (pSouth->emptySpace())) {

            bool isFloor = (pSouth->getSpace() == SPACE::FLOOR);
            this->newRamp(isFloor, CARDINAL::NORTH);
        }

    } else if (space == SPACE::RAMP_FEW) {
        if ((pEast != nullptr) && (pEast->emptySpace())) {

            bool isFloor = (pEast->getSpace() == SPACE::FLOOR);
            this->newRamp(isFloor, CARDINAL::WEST);

        } else if ((pWest != nullptr) && (pWest->emptySpace())) {

            bool isFloor = (pWest->getSpace() == SPACE::FLOOR);
            this->newRamp(isFloor, CARDINAL::EAST);
        }
    }
}

void Cube::create(Mesh* mesh) {
    this->mesh = mesh;

    SPACE val = this->getSpace();
    switch (val) {
        case SPACE::EMPTY:
            this->newWall();
            break;
        case SPACE::FLOOR: {
            this->newWall();
            this->newFloor();
        } break;
        case SPACE::CEILING: {
            this->newWall();
            this->newCeeling();
        } break;
        case SPACE::FC: {
            this->newWall();
            this->newFloor();
            this->newCeeling();
        } break;
        case SPACE::DIAG: {
            this->newDiag();
        } break;
        case SPACE::RAMP_FNS:
        case SPACE::RAMP_FEW:
            this->newRampNSEW(val);
            break;
        default:
            break;
    }
}
} // namespace Chimera