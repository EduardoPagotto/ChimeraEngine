#pragma once
#include "Cube.hpp"
#include <stdint.h>
#include <vector>

namespace Chimera {

class Maze {
  public:
    Maze(const char filename[], Mesh* mesh, float sizeBlock);
    virtual ~Maze();
    void createMap();

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

    float sizeBlock;
    glm::ivec3 size;
    glm::vec3 halfBlock;
    std::vector<Cube*> vpCube;
    Mesh* mesh;
};
} // namespace Chimera
