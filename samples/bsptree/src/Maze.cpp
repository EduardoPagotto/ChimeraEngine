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

    // carregando mapa
    for (uint32_t h = 0; h < this->height; h++) {
        fgets(string, 1024, file);
        for (uint32_t w = 0; w < this->width; w++) {
            int indice = w + (h * this->width);
            if (string[w] == 0x20) {
                // Bloco Vazio
                this->data.push_back(0); // this->data[indice] = 0;
            } else {
                // parede (conversão ASCII completa)
                uint8_t val = (uint8_t)(string[w] - 0x30);
                this->data.push_back(val); // this->data[indice] = val;
            }
        }
    }
}

Maze::~Maze() { this->data.clear(); }

uint32_t Maze::getIndexVal(uint32_t w, uint32_t h) { return w + (h * this->width); }

void Maze::newWall(CARDINAL cardinal, uint32_t w, uint32_t h) {

    uint8_t val_target = 0;
    std::vector<Chimera::VertexData> vl;

    float sizeBlock = 10.0f;

    float y_max = sizeBlock / 2; // Altura Maxima
    float y_min = -y_max;        // Altura Minima

    float halfSizeX = (this->width * sizeBlock) / 2.0f;
    float halfSizeZ = (this->height * sizeBlock) / 2.0f;

    // float x_max; // para East  (width++)
    // float x_min; // para Oest  (width++)
    // float z_max; // para South (height++)
    // float z_min; // para North (height--)

    switch (cardinal) {
        case CARDINAL::NORTH:
            if (h != 0) {
                val_target = this->data[this->getIndexVal(w, h - 1)];
                if (val_target == 1) {

                    float x_min = (w * sizeBlock) - halfSizeX;
                    float x_max = x_min + sizeBlock;
                    float z_min = ((h - 1) * halfSizeX) - halfSizeZ;
                    float z_max = z_min + sizeBlock;

                    glm::vec3 norm = glm::vec3(0.0f, 0.0f, 1.0f);
                    vl.push_back({glm::vec3(x_min, y_min, z_min), norm, glm::vec2(0, 0)}); // np0
                    vl.push_back({glm::vec3(x_max, y_min, z_min), norm, glm::vec2(1, 0)}); // np1
                    vl.push_back({glm::vec3(x_max, y_max, z_min), norm, glm::vec2(1, 1)}); // np2
                    vl.push_back({glm::vec3(x_min, y_max, z_min), norm, glm::vec2(0, 1)}); // np3
                }
            }
            break;
        case CARDINAL::EAST:
            if (w != this->width - 1) {
                val_target = this->data[this->getIndexVal(w + 1, h)];
                if (val_target == 1) {

                    float x_min = ((w + 1) * sizeBlock) - halfSizeX;
                    float x_max = x_min + sizeBlock;
                    float z_min = (h * halfSizeX) - halfSizeZ;
                    float z_max = z_min + sizeBlock;

                    glm::vec3 norm = glm::vec3(1.0f, 0.0f, 0.0f);
                    vl.push_back({glm::vec3(x_max, y_min, z_min), norm, glm::vec2(0, 0)}); // ep0
                    vl.push_back({glm::vec3(x_max, y_min, z_max), norm, glm::vec2(1, 0)}); // ep1
                    vl.push_back({glm::vec3(x_max, y_max, z_max), norm, glm::vec2(1, 1)}); // ep2
                    vl.push_back({glm::vec3(x_max, y_max, z_min), norm, glm::vec2(0, 1)}); // ep3
                }
            }
            break;
        case CARDINAL::SOUTH:
            if (h != this->height - 1) {
                val_target = this->data[this->getIndexVal(w, h + 1)];
                if (val_target == 1) {

                    float x_min = (w * sizeBlock) - halfSizeX;
                    float x_max = x_min + sizeBlock;
                    float z_min = ((h + 1) * halfSizeX) - halfSizeZ;
                    float z_max = z_min + sizeBlock;

                    glm::vec3 norm = glm::vec3(0.0f, 0.0f, -1.0f);
                    vl.push_back({glm::vec3(x_min, y_min, z_max), norm, glm::vec2(1, 0)}); // np0
                    vl.push_back({glm::vec3(x_max, y_min, z_max), norm, glm::vec2(0, 0)}); // np1
                    vl.push_back({glm::vec3(x_max, y_max, z_max), norm, glm::vec2(0, 1)}); // np2
                    vl.push_back({glm::vec3(x_min, y_max, z_max), norm, glm::vec2(1, 1)}); // np3
                }
            }
            break;
        case CARDINAL::WEST:
            if (w != 0) {
                val_target = this->data[this->getIndexVal(w - 1, h)];
                if (val_target == 1) {

                    float x_min = ((w - 1) * sizeBlock) - halfSizeX;
                    float x_max = x_min + sizeBlock;
                    float z_min = (h * halfSizeX) - halfSizeZ;
                    float z_max = z_min + sizeBlock;

                    glm::vec3 norm = glm::vec3(-1.0f, 0.0f, 0.0f);
                    vl.push_back({glm::vec3(x_min, y_min, z_min), norm, glm::vec2(1, 0)}); // ep0
                    vl.push_back({glm::vec3(x_min, y_min, z_max), norm, glm::vec2(0, 0)}); // ep1
                    vl.push_back({glm::vec3(x_min, y_max, z_max), norm, glm::vec2(0, 1)}); // ep2
                    vl.push_back({glm::vec3(x_min, y_max, z_min), norm, glm::vec2(1, 1)}); // ep3
                }
            }
            break;
    }

    if (vl.size() > 0) {
        this->vertexData.push_back(vl[0]);
        this->vertexData.push_back(vl[1]);
        this->vertexData.push_back(vl[2]);
        this->vertexData.push_back(vl[2]);
        this->vertexData.push_back(vl[3]);
        this->vertexData.push_back(vl[0]);
    }
}

void Maze::createMap() {

    for (uint32_t h = 0; h < this->height; h++) {
        for (uint32_t w = 0; w < this->width; w++) {
            uint8_t val = this->data[this->getIndexVal(w, h)];
            if (val == 0) {
                for (uint8_t i = (uint8_t)CARDINAL::NORTH; i <= (uint8_t)CARDINAL::WEST; i++) {
                    this->newWall((CARDINAL)i, w, h);
                }
            }
        }
    }
}