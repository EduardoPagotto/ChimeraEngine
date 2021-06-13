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
    NORTH_WEST = 7, // LEFT DOWN
    NONE = 8
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
    inline uint32_t getIndexArrayPos(const glm::ivec3& pos) const {
        return pos.x + (pos.z * this->size.x) + (pos.y * this->size.x * this->size.z);
    }

    inline bool valid(const glm::ivec3& val) const {
        return ((val.z >= 0) && (val.z < this->size.z) && (val.x >= 0) && (val.x < this->size.x) && (val.y >= 0) && (val.y < this->size.y));
    }

    inline bool empty(const SPACE& s) const {
        return ((s == SPACE::EMPTY) || (s == SPACE::FLOOR) || (s == SPACE::CEILING) || (s == SPACE::FC));
    }

    CARDINAL emptyQuadrantDiag(bool invert, const glm::ivec3& pos);
    void newWall(const glm::ivec3& pos);
    void newDiag(CARDINAL card, const glm::ivec3& pos);
    void newFloor(bool clockwise, CARDINAL card, const glm::ivec3& pos);
    void newCeeling(bool clockwise, CARDINAL card, const glm::ivec3& pos);
    glm::vec3 minimal(const glm::ivec3& pos) const;
    void makeFaceSquare(bool clockwise, std::vector<Chimera::VertexData>& vl);
    void makeFaceTriangle(bool clockwise, std::vector<Chimera::VertexData>& vl);
    inline SPACE getCardinal(const glm::ivec3& pos) const { return this->data[this->getIndexArrayPos(pos)]; }
    glm::ivec3 getCardinalPos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos);
    SPACE getCardinalNeighbor(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos);

    bool hasNeighbor(CARDINAL card, SPACE space, const glm::ivec3& pos);

    glm::ivec3 size;
    float sizeBlock;
    glm::vec3 halfBlock;
    uint32_t indexPointCount;
    std::vector<SPACE> data;
    std::vector<Chimera::Triangle> trisList;
};

#endif