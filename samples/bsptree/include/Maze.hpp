#ifndef __GAME_TESTE_MAZE__HPP
#define __GAME_TESTE_MAZE__HPP

#include "chimera/render/Triangle.hpp"
#include "chimera/render/vbs/VertexData.hpp"
#include <glm/glm.hpp>
#include <stdint.h>
#include <vector>

enum class CARDINAL { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

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
    void newFloor(uint32_t d, uint32_t w, uint32_t h);
    void newCeeling(uint32_t d, uint32_t w, uint32_t h);
    glm::vec3 minimal(uint32_t d, uint32_t w, uint32_t h);
    void makeFace(bool clockwise, std::vector<Chimera::VertexData>& vl);

    uint32_t width;  // Largura (colunas)
    uint32_t height; // Altura (linhas)
    uint32_t deep;   // andar
    float sizeBlock;
    float halfSizeX;
    float halfSizeZ;
    float halfSizeY;
    std::vector<uint8_t> data;
    std::vector<Chimera::Triangle> trisList;
};

#endif