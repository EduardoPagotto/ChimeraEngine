#ifndef __GAME_TESTE_MAZE__HPP
#define __GAME_TESTE_MAZE__HPP

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

  private:
    uint32_t getIndexVal(uint32_t w, uint32_t h);

    void newWall(CARDINAL cardinal, uint32_t w, uint32_t h);

    uint32_t width;  // Largura (colunas)
    uint32_t height; // Altura (linhas)
    std::vector<uint8_t> data;

    std::vector<Chimera::VertexData> vertexData;
};

#endif