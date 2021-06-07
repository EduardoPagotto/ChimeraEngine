#ifndef __GAME_TESTE_MAZE__HPP
#define __GAME_TESTE_MAZE__HPP

#include "chimera/render/Triangle.hpp"
#include "chimera/render/vbs/VertexData.hpp"
#include <glm/glm.hpp>
#include <stdint.h>
#include <vector>

// enum class CARDINAL { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };
enum class CARDINAL {
    NORTH = 0,      // UP
    NORTH_EAST = 1, // RIGHT UP
    EAST = 2,       // RIGHT
    SOUTH_EAST = 3, // RIGHT DOWN
    SOUTH = 4,      // DOWN
    SOUTH_WEST = 5, // LEFT UP
    WEST = 6,       // LEFT
    NORTH_WEST = 7  // LEFT DOWN
};

enum class DEEP {
    UP = 0,     // ABOVE
    MIDDLE = 1, // LEVEL
    BOTTOM = 2  // UNDER
};

enum class SPACE { EMPTY = 0, WALL = 1, DIAG = 2, FLOOR = 3, CEILING = 4, FC = 5, RAMP = 6, INVALID = 99 };

class Maze {
  public:
    Maze(const char filename[]);
    virtual ~Maze();

    void createMap();

    std::vector<Chimera::VertexData> vertexData;
    std::vector<unsigned int> vIndex;

  private:
    uint32_t getIndexVal(uint32_t d, uint32_t w, uint32_t h);

    void newWall(CARDINAL cardinal, uint32_t d, uint32_t w, uint32_t h);
    void newFloor(bool clockwise, uint8_t quad, uint32_t d, uint32_t w, uint32_t h);
    void newCeeling(bool clockwise, uint8_t quad, uint32_t d, uint32_t w, uint32_t h);
    glm::vec3 minimal(uint32_t d, uint32_t w, uint32_t h);
    void makeFaceSquare(bool clockwise, std::vector<Chimera::VertexData>& vl);
    void makeFaceTriangle(bool clockwise, std::vector<Chimera::VertexData>& vl);

    SPACE getCardinalNeighbor(DEEP deep, CARDINAL card, glm::ivec3 dist, glm::ivec3 pos);

    uint32_t width;  // Largura (colunas)
    uint32_t height; // Altura (linhas)
    uint32_t deep;   // andar
    float sizeBlock;
    float halfSizeX;
    float halfSizeZ;
    float halfSizeY;
    uint32_t indexPointCount;
    std::vector<SPACE> data;
    std::vector<Chimera::Triangle> trisList;
};

#endif