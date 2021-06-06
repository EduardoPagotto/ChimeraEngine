#include "Maze.hpp"
#include "chimera/core/Exception.hpp"
#include <string>

Maze::Maze(const char filename[]) {

    FILE* file;
    char string[1024];

    file = fopen(filename, "rb");
    if (!file) {
        throw Chimera::Exception(std::string("Arguivo nao localizado"));
    }

    // tamanho do mapa
    fgets(string, 1024, file);
    this->width = atoi(string);

    fgets(string, 1024, file);
    this->height = atoi(string);

    fgets(string, 1024, file);
    this->deep = atoi(string);

    // carregando mapa
    for (uint32_t d = 0; d < this->deep; d++) {
        for (uint32_t h = 0; h < this->height; h++) {

            fgets(string, 1024, file);

            for (uint32_t w = 0; w < this->width; w++) {

                int indice = getIndexVal(d, w, h);
                if (string[w] == 0x20) {
                    // Bloco Vazio
                    this->data.push_back(SPACE::EMPTY);
                } else {
                    // parede (conversão ASCII completa)
                    SPACE val = (SPACE)(string[w] - 0x30);
                    this->data.push_back(val);
                }
            }
        }
    }

    this->sizeBlock = 10.0f;
    this->halfSizeX = (this->width * sizeBlock) / 2.0f;
    this->halfSizeZ = (this->height * sizeBlock) / 2.0f;
    this->halfSizeY = (this->deep * sizeBlock) / 2.0f;
}

Maze::~Maze() { this->data.clear(); }

uint32_t Maze::getIndexVal(uint32_t d, uint32_t w, uint32_t h) { return w + (h * this->width) + (d * this->width * this->height); }

glm::vec3 Maze::minimal(uint32_t d, uint32_t w, uint32_t h) {

    float x_min = (w * sizeBlock) - halfSizeX; // para Oest  (width++)
    float y_min = (d * sizeBlock) - halfSizeY; // Altura Minima andar
    float z_min = (h * sizeBlock) - halfSizeZ; // para North (height--)

    return glm::vec3(x_min, y_min, z_min);
}

void Maze::newWall(CARDINAL cardinal, uint32_t d, uint32_t w, uint32_t h) {

    SPACE val_target;
    bool clockwise = true;
    std::vector<Chimera::VertexData> vl;

    glm::vec3 min = this->minimal(d, w, h);
    glm::vec3 max = min + sizeBlock; // x->East (width++); //y->Altura Maxima andar //z:>South (height++)

    switch (cardinal) {
        case CARDINAL::NORTH:
            if (h != 0) {
                val_target = this->data[this->getIndexVal(d, w, h - 1)];
                if (val_target == SPACE::WALL) {
                    // N
                    clockwise = false;
                    vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // n p0
                    vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // n p1
                    vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // n p2
                    vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // n p3
                }
            }
            break;
        case CARDINAL::EAST:
            if (w != this->width - 1) {
                val_target = this->data[this->getIndexVal(d, w + 1, h)];
                if (val_target == SPACE::WALL) {
                    // E
                    clockwise = false;
                    vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // ep0
                    vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // ep1
                    vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // ep2
                    vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // ep3

                } else if (val_target == SPACE::DIAG) {

                    SPACE north = this->data[this->getIndexVal(d, w + 1, h - 1)];
                    SPACE east = this->data[this->getIndexVal(d, w + 2, h)];
                    SPACE south = this->data[this->getIndexVal(d, w + 1, h + 1)];

                    if ((north == SPACE::WALL) && (east == SPACE::WALL)) {

                        // ne (diag. sup. dir.)
                        max.x += sizeBlock;
                        min.x += sizeBlock;
                        clockwise = false;
                        vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // en0
                        vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // en1
                        vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // en2
                        vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // en3

                    } else if ((south == SPACE::WALL) && (east == SPACE::WALL)) {
                        // se (diag. inf. dir.)
                        max.x += sizeBlock;
                        min.x += sizeBlock;
                        clockwise = false;
                        vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // ep0
                        vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // ep1
                        vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // ep2
                        vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // ep3
                    }
                }
            }
            break;
        case CARDINAL::SOUTH:
            if (h != this->height - 1) {
                val_target = this->data[this->getIndexVal(d, w, h + 1)];
                if (val_target == SPACE::WALL) {
                    // S
                    vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // np0
                    vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // np1
                    vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // np2
                    vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // np3
                }
            }
            break;
        case CARDINAL::WEST:
            if (w != 0) {
                val_target = this->data[this->getIndexVal(d, w - 1, h)];
                if (val_target == SPACE::WALL) {
                    // W
                    vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // ep0
                    vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // ep1
                    vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // ep2
                    vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // ep3
                } else if (val_target == SPACE::DIAG) {

                    SPACE north = this->data[this->getIndexVal(d, w - 1, h - 1)];
                    SPACE west = this->data[this->getIndexVal(d, w - 2, h)];
                    SPACE south = this->data[this->getIndexVal(d, w - 1, h + 1)];

                    if ((south == SPACE::WALL) && (west == SPACE::WALL)) {
                        // sw (diag. inf. esq.)
                        max.x -= sizeBlock;
                        min.x -= sizeBlock;
                        vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // ep0
                        vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // ep1
                        vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // ep2
                        vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // ep3
                    } else if ((north == SPACE::WALL) && (west == SPACE::WALL)) {
                        // nw (diag. sup. esq.)
                        max.x -= sizeBlock;
                        min.x -= sizeBlock;
                        vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // ep0
                        vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // ep1
                        vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // ep2
                        vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // ep3
                    }
                }
            }
            break;
    }

    if (vl.size() > 0)
        this->makeFaceSquare(clockwise, vl);
}

void Maze::makeFaceTriangle(bool clockwise, std::vector<Chimera::VertexData>& vl) {
    int pa, pb, pc, pd;
    if (!clockwise) {
        pa = this->trisList.size() * 2;
        pb = pa + 1;
        pc = pa + 2;
    } else {
        pc = this->trisList.size() * 2;
        pb = pa + 1;
        pa = pa + 2;
    }

    this->vertexData.push_back(vl[0]);
    this->vertexData.push_back(vl[1]);
    this->vertexData.push_back(vl[2]);

    Chimera::Triangle t1 = Chimera::Triangle(pa, pb, pc, glm::vec3(0.0f));
    t1.calcNormal(this->vertexData);
    this->trisList.push_back(t1);
}

void Maze::makeFaceSquare(bool clockwise, std::vector<Chimera::VertexData>& vl) {
    int pa, pb, pc, pd;
    pa = this->trisList.size() * 2;
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
}

void Maze::newFloor(uint32_t d, uint32_t w, uint32_t h) {

    std::vector<Chimera::VertexData> vl;
    glm::vec3 min = this->minimal(d, w, h);
    glm::vec3 max = min + sizeBlock; // x->East (width++); //y->Altura Maxima andar //z:>South (height++)

    vl.push_back({glm::vec3(min.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // n p0
    vl.push_back({glm::vec3(max.x, min.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // n p1
    vl.push_back({glm::vec3(max.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // n p2
    vl.push_back({glm::vec3(min.x, min.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // n p3

    this->makeFaceSquare(false, vl);
}

void Maze::newCeeling(uint32_t d, uint32_t w, uint32_t h) {

    std::vector<Chimera::VertexData> vl;
    glm::vec3 min = this->minimal(d, w, h);
    glm::vec3 max = min + sizeBlock; // x->East (width++); //y->Altura Maxima andar //z:>South (height++)

    vl.push_back({glm::vec3(min.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(1, 0)}); // n p0
    vl.push_back({glm::vec3(max.x, max.y, max.z), glm::vec3(0.0f), glm::vec2(0, 0)}); // n p1
    vl.push_back({glm::vec3(max.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(0, 1)}); // n p2
    vl.push_back({glm::vec3(min.x, max.y, min.z), glm::vec3(0.0f), glm::vec2(1, 1)}); // n p3

    this->makeFaceSquare(true, vl);
}

void Maze::createMap() {

    for (uint32_t d = 0; d < this->deep; d++) {
        for (uint32_t h = 0; h < this->height; h++) {
            for (uint32_t w = 0; w < this->width; w++) {
                SPACE val = this->data[this->getIndexVal(d, w, h)];
                if ((val == SPACE::EMPTY) || (val == SPACE::FLOOR) || (val == SPACE::CEILING) || (val == SPACE::FC)) {
                    for (uint8_t i = (uint8_t)CARDINAL::NORTH; i <= (uint8_t)CARDINAL::WEST; i++) {
                        this->newWall((CARDINAL)i, d, w, h);
                    }
                }

                if (val == SPACE::FLOOR) {
                    this->newFloor(d, w, h);
                } else if (val == SPACE::CEILING) {
                    this->newCeeling(d, w, h);
                } else if (val == SPACE::FC) {
                    this->newFloor(d, w, h);
                    this->newCeeling(d, w, h);
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