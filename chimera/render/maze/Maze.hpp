#ifndef __GAME_TESTE_MAZE__HPP
#define __GAME_TESTE_MAZE__HPP

#include "Cube.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/space/Triangle.hpp"
#include <glm/glm.hpp>
#include <stdint.h>
#include <vector>

namespace Chimera {

class Maze {
  public:
    Maze(const char filename[]);
    virtual ~Maze();

    void createMap();

    std::vector<VertexData> vertexData;
    std::vector<unsigned int> vIndex;

  private:
    inline uint32_t getIndexArrayPos(const glm::ivec3& pos) const {
        return pos.x + (pos.z * this->size.x) + (pos.y * this->size.x * this->size.z);
    }

    inline bool valid(const glm::ivec3& val) const {
        return ((val.z >= 0) && (val.z < this->size.z) && (val.x >= 0) && (val.x < this->size.x) && (val.y >= 0) && (val.y < this->size.y));
    }

    glm::vec3 minimal(const glm::ivec3& pos) const;
    glm::ivec3 getCardinalPos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos);

    Cube* getCubeNeighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos);

    glm::ivec3 size;
    float sizeBlock;
    glm::vec3 halfBlock;
    uint32_t indexPointCount;
    std::vector<Cube*> vpCube;
    std::vector<Core::Triangle> trisList;
};
} // namespace Chimera
#endif