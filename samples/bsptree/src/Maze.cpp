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

uint32_t Maze::getIndexVal(uint8_t w, uint8_t h) { return w + (h * this->width); }

uint8_t Maze::newWall(CARDINAL cardinal, uint8_t w, uint8_t h) {

    uint8_t val_target = 0;
    glm::vec3 norm = glm::vec3(0.0);
    std::vector<Chimera::VertexData> vl;

    float altura_max = 3.0f;
    float altura_min = 0.0f;

    float x, y;

    float x_max = x - 9.5f;
    float x_min = x - 10.5f;

    float y_max = (20.0f - y) + 0.5f;
    float y_min = (20.0f - y) - 0.5f;

    switch (cardinal) {
        case CARDINAL::NORTH:
            if (h != 0) {
                val_target = this->data[this->getIndexVal(w, h - 1)];
                norm = glm::vec3(0.0f, 1.0f, 0.0f);
                vl.push_back({glm::vec3(x_min, altura_max, y_min), norm, glm::vec2(0, 0)}); // np0
                vl.push_back({glm::vec3(x_max, altura_max, y_min), norm, glm::vec2(1, 0)}); // np1
                vl.push_back({glm::vec3(x_max, altura_min, y_min), norm, glm::vec2(1, 1)}); // np2
                vl.push_back({glm::vec3(x_min, altura_min, y_min), norm, glm::vec2(0, 1)}); // np3
            }
            break;
        case CARDINAL::EAST:
            if (w != this->width - 1) {
                val_target = this->data[this->getIndexVal(w + 1, h)];
                norm = glm::vec3(1.0f, 0.0f, 0.0f);
                vl.push_back({glm::vec3(x_max, altura_max, y_min), norm, glm::vec2(0, 0)}); // ep0
                vl.push_back({glm::vec3(x_max, altura_max, y_max), norm, glm::vec2(1, 0)}); // ep1
                vl.push_back({glm::vec3(x_max, altura_min, y_max), norm, glm::vec2(1, 1)}); // ep2
                vl.push_back({glm::vec3(x_max, altura_min, y_min), norm, glm::vec2(0, 1)}); // ep3
            }
            break;
        case CARDINAL::SOUTH:
            if (h != this->height - 1) {
                val_target = this->data[this->getIndexVal(w, h + 1)];
                norm = glm::vec3(0.0f, -1.0f, 0.0f);
                vl.push_back({glm::vec3(x_max, altura_max, y_max), norm, glm::vec2(0, 0)}); // sp0
                vl.push_back({glm::vec3(x_min, altura_max, y_max), norm, glm::vec2(1, 0)}); // sp1
                vl.push_back({glm::vec3(x_min, altura_min, y_max), norm, glm::vec2(1, 1)}); // sp2
                vl.push_back({glm::vec3(x_max, altura_min, y_max), norm, glm::vec2(0, 1)}); // sp3
            }
            break;
        case CARDINAL::WEST:
            if (w == 0) {
                val_target = this->data[this->getIndexVal(w - 1, h)];
                norm = glm::vec3(-1.0f, 1.0f, 0.0f);
                vl.push_back({glm::vec3(x_min, altura_max, y_max), norm, glm::vec2(0, 0)}); // wp0
                vl.push_back({glm::vec3(x_min, altura_max, y_min), norm, glm::vec2(1, 0)}); // wp1
                vl.push_back({glm::vec3(x_min, altura_min, y_min), norm, glm::vec2(1, 1)}); // wp2
                vl.push_back({glm::vec3(x_min, altura_min, y_max), norm, glm::vec2(0, 1)}); // wp3
            }
            break;
    }

    this->vertexData.push_back(vl[0]);
    this->vertexData.push_back(vl[1]);
    this->vertexData.push_back(vl[2]);
    this->vertexData.push_back(vl[2]);
    this->vertexData.push_back(vl[3]);
    this->vertexData.push_back(vl[0]);

    return val_target;
}

void Maze::createMap() {

    for (uint32_t h = 0; h < this->height; h++) {
        for (uint32_t w = 0; w < this->width; w++) {
            uint8_t val = this->data[this->getIndexVal(w, h)];
            if (val == 0) {

                for (uint8_t i = (uint8_t)CARDINAL::NORTH; i <= (uint8_t)CARDINAL::WEST; i++) {
                    uint8_t val = this->newWall((CARDINAL)i, w, h);
                }
            }
        }
    }
}