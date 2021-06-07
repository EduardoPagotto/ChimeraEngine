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

    // carregando mapa
    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            fgets(string, 1024, file);
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {
                if (string[pos.x] == 0x20) {
                    // Bloco Vazio
                    this->data.push_back(SPACE::EMPTY);
                } else {
                    // parede (conversão ASCII completa)
                    SPACE val = (SPACE)(string[pos.x] - 0x30);
                    this->data.push_back(val);
                }
            }
        }
    }

    this->sizeBlock = 10.0f;

    this->halfBlock.x = (this->size.x * sizeBlock) / 2.0f; //(w/2)
    this->halfBlock.y = (this->size.y * sizeBlock) / 2.0f; //(d/2)
    this->halfBlock.z = (this->size.z * sizeBlock) / 2.0f; //(h/2)
}

Maze::~Maze() { this->data.clear(); }

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

void Maze::newWall(CARDINAL cardinal, const glm::ivec3& pos) {

    SPACE val_target;
    bool clockwise = true;
    std::vector<Chimera::VertexData> vl;

    glm::vec3 min = this->minimal(pos);
    glm::vec3 max = min + sizeBlock; // x->East (width++); //y->Altura Maxima andar //z:>South (height++)

    switch (cardinal) {
        case CARDINAL::NORTH: {
            val_target = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, glm::ivec3(1), pos);
            if (val_target == SPACE::WALL) {
                // N
                clockwise = false;
                vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // n p0
                vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // n p1
                vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // n p2
                vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // n p3
            }
        } break;
        case CARDINAL::EAST: {
            val_target = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos);
            if (val_target == SPACE::WALL) {
                // E
                clockwise = false;
                vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // e p0
                vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // e p1
                vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // e p2
                vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // e p3

            } else if (val_target == SPACE::DIAG) {
                SPACE north = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH_EAST, glm::ivec3(1), pos);
                SPACE east = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(2), pos);
                SPACE south = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH_EAST, glm::ivec3(1), pos);

                if ((north == SPACE::WALL) && (east == SPACE::WALL)) {

                    // ne (diag. sup. dir.)
                    max.x += sizeBlock;
                    min.x += sizeBlock;
                    clockwise = false;
                    vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // en p0
                    vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // en p1
                    vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // en p2
                    vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // en p3

                    SPACE cf = this->getCardinal(pos);
                    glm::ivec3 new_pos = this->getCardinalPos(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos);
                    if (cf == SPACE::CEILING) {
                        this->newCeeling(false, 0, new_pos);
                    } else if (cf == SPACE::FLOOR) {
                        this->newFloor(false, 0, new_pos);
                    } else if (cf == SPACE::FC) {
                        this->newCeeling(false, 0, new_pos);
                        this->newFloor(false, 0, new_pos);
                    }

                } else if ((south == SPACE::WALL) && (east == SPACE::WALL)) {
                    // se (diag. inf. dir.)
                    max.x += sizeBlock;
                    min.x += sizeBlock;
                    clockwise = false;
                    vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // se p0
                    vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // se p1
                    vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // se p2
                    vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // se p3

                    SPACE cf = this->getCardinal(pos);
                    glm::ivec3 new_pos = this->getCardinalPos(DEEP::MIDDLE, CARDINAL::EAST, glm::ivec3(1), pos);
                    if (cf == SPACE::CEILING) {
                        this->newCeeling(false, 1, new_pos);
                    } else if (cf == SPACE::FLOOR) {
                        this->newFloor(false, 1, new_pos);
                    } else if (cf == SPACE::FC) {
                        this->newCeeling(false, 1, new_pos);
                        this->newFloor(false, 1, new_pos);
                    }
                }
            }
        } break;
        case CARDINAL::SOUTH: {
            val_target = getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, glm::ivec3(1), pos);
            if (val_target == SPACE::WALL) {
                // S
                vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // s p0
                vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // s p1
                vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // s p2
                vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // s p3
            }
        } break;
        case CARDINAL::WEST: {
            val_target = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos);
            if (val_target == SPACE::WALL) {
                // W
                vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // w p0
                vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // w p1
                vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // w p2
                vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // w p3
            } else if (val_target == SPACE::DIAG) {
                SPACE north = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::NORTH_WEST, glm::ivec3(1), pos);
                SPACE west = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(2), pos);
                SPACE south = this->getCardinalNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH_WEST, glm::ivec3(1), pos);

                if ((south == SPACE::WALL) && (west == SPACE::WALL)) {
                    // sw (diag. inf. esq.)
                    max.x -= sizeBlock;
                    min.x -= sizeBlock;
                    vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // sw p0
                    vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // sw p1
                    vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // sw p2
                    vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // sw p3

                    SPACE cf = this->getCardinal(pos);
                    glm::ivec3 new_pos = this->getCardinalPos(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos);
                    if (cf == SPACE::CEILING) {
                        this->newCeeling(false, 2, new_pos);
                    } else if (cf == SPACE::FLOOR) {
                        this->newFloor(false, 2, new_pos);
                    } else if (cf == SPACE::FC) {
                        this->newCeeling(false, 2, new_pos);
                        this->newFloor(false, 2, new_pos);
                    }

                } else if ((north == SPACE::WALL) && (west == SPACE::WALL)) {
                    // nw (diag. sup. esq.)
                    max.x -= sizeBlock;
                    min.x -= sizeBlock;
                    vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // nw p0
                    vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // nw p1
                    vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // nw p2
                    vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // nw p3

                    SPACE cf = this->getCardinal(pos);
                    glm::ivec3 new_pos = this->getCardinalPos(DEEP::MIDDLE, CARDINAL::WEST, glm::ivec3(1), pos);
                    if (cf == SPACE::CEILING) {
                        this->newCeeling(false, 3, new_pos);
                    } else if (cf == SPACE::FLOOR) {
                        this->newFloor(false, 3, new_pos);
                    } else if (cf == SPACE::FC) {
                        this->newCeeling(false, 3, new_pos);
                        this->newFloor(false, 3, new_pos);
                    }
                }
            }
        } break;
    }

    if (vl.size() > 0)
        this->makeFaceSquare(clockwise, vl);
}

void Maze::makeFaceTriangle(bool clockwise, std::vector<Chimera::VertexData>& vl) {
    int pa, pb, pc, pd;
    if (!clockwise) {
        pa = this->indexPointCount;
        pb = pa + 1;
        pc = pa + 2;
    } else {
        pc = this->indexPointCount;
        pb = pa + 1;
        pa = pa + 2;
    }

    this->vertexData.push_back(vl[0]);
    this->vertexData.push_back(vl[1]);
    this->vertexData.push_back(vl[2]);

    Chimera::Triangle t1 = Chimera::Triangle(pa, pb, pc, glm::vec3(0.0f));
    t1.calcNormal(this->vertexData);
    this->trisList.push_back(t1);

    this->indexPointCount += 3;
}

void Maze::makeFaceSquare(bool clockwise, std::vector<Chimera::VertexData>& vl) {
    int pa, pb, pc, pd;
    pa = this->indexPointCount;
    if (!clockwise) {
        pb = pa + 1;
        pc = pa + 2;
        pd = pa + 3;
    } else {
        pb = pa + 3;
        pc = pa + 2;
        pd = pa + 1;
    }

    this->vertexData.push_back(vl[0]);
    this->vertexData.push_back(vl[1]);
    this->vertexData.push_back(vl[2]);
    this->vertexData.push_back(vl[3]);

    Chimera::Triangle t1 = Chimera::Triangle(pa, pb, pc, glm::vec3(0.0f));
    Chimera::Triangle t2 = Chimera::Triangle(pc, pd, pa, glm::vec3(0.0f));

    t1.calcNormal(this->vertexData);
    t2.calcNormal(this->vertexData);

    this->trisList.push_back(t1);
    this->trisList.push_back(t2);

    this->indexPointCount += 4;
}

void Maze::newFloor(bool clockwise, uint8_t quad, const glm::ivec3& pos) {

    std::vector<Chimera::VertexData> vl;
    glm::vec3 min = this->minimal(pos);
    glm::vec3 max = min + sizeBlock; // x->East (width++); //y->Altura Maxima andar //z:>South (height++)

    if (quad == 0) {

        vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // f p0
        vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // f p1
        vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // f p3
        this->makeFaceTriangle(clockwise, vl);

    } else if (quad == 1) {

        vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // f p0
        vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // f p2
        vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // f p3
        this->makeFaceTriangle(clockwise, vl);

    } else if (quad == 2) {
        vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // f p1
        vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // f p2
        vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // f p3
        this->makeFaceTriangle(clockwise, vl);

    } else if (quad == 3) {

        vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // f p0
        vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // f p1
        vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // f p2
        this->makeFaceTriangle(clockwise, vl);

    } else {

        vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // f p0
        vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // f p1
        vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // f p2
        vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // f p3
        this->makeFaceSquare(clockwise, vl);
    }
}

void Maze::newCeeling(bool clockwise, uint8_t quad, const glm::ivec3& pos) {

    std::vector<Chimera::VertexData> vl;
    glm::vec3 min = this->minimal(pos);
    glm::vec3 max = min + sizeBlock; // x->East (width++); //y->Altura Maxima andar //z:>South (height++)

    if (quad == 0) {

        vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // c p0
        vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // c p1
        vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // c p3
        this->makeFaceTriangle(clockwise, vl);

    } else if (quad == 1) {

        vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // c p0
        vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // c p2
        vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // c p3
        this->makeFaceTriangle(clockwise, vl);

    } else if (quad == 2) {

        vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // c p1
        vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // c p2
        vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // c p3
        this->makeFaceTriangle(clockwise, vl);

    } else if (quad == 3) {

        vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // c p0
        vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // c p1
        vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // c p2
        this->makeFaceTriangle(clockwise, vl);

    } else {

        vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // c p0
        vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // c p1
        vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // c p2
        vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // c p3
        this->makeFaceSquare(clockwise, vl);
    }
}
// // lembrar d = y;  w = x;  h = z;
void Maze::createMap() {
    glm::ivec3 pos;
    for (pos.y = 0; pos.y < this->size.y; pos.y++) {
        for (pos.z = 0; pos.z < this->size.z; pos.z++) {
            for (pos.x = 0; pos.x < this->size.x; pos.x++) {

                SPACE val = this->getCardinal(pos);
                if ((val == SPACE::EMPTY) || (val == SPACE::FLOOR) || (val == SPACE::CEILING) || (val == SPACE::FC)) {
                    this->newWall(CARDINAL::NORTH, pos);
                    this->newWall(CARDINAL::EAST, pos);
                    this->newWall(CARDINAL::SOUTH, pos);
                    this->newWall(CARDINAL::WEST, pos);
                }

                if (val == SPACE::FLOOR) {
                    this->newFloor(false, 5, pos);
                } else if (val == SPACE::CEILING) {
                    this->newCeeling(true, 5, pos);
                } else if (val == SPACE::FC) {
                    this->newFloor(false, 5, pos);
                    this->newCeeling(true, 5, pos);
                }
            }
        }

        for (Chimera::Triangle t : this->trisList) {
            vIndex.push_back(t.p[0]);
            vIndex.push_back(t.p[1]);
            vIndex.push_back(t.p[2]);
        }
    }
}