#ifndef __GAME_TESTE_CUBE__HPP
#define __GAME_TESTE_CUBE__HPP

#include "chimera/render/Triangle.hpp"
#include "chimera/render/vbs/VertexData.hpp"
#include <glm/glm.hpp>
#include <vector>

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

class Cube {
  public:
    Cube(const SPACE& space, const glm::ivec3& pos, const glm::vec3& min, const glm::vec3& max);
    virtual ~Cube();

    void addFace(bool clockwise, int num);

    inline SPACE getSpace() const { return this->space; }

    // Cube* pNorth;
    // Cube* pEast;
    // Cube* pSouth;
    // Cube* pWest;

  private:
    uint32_t indexPointCount;
    SPACE space;
    glm::ivec3 position;
    glm::vec3 p[8];
    glm::vec2 t[4];
    glm::vec3 max;
    glm::vec3 min;
    std::vector<glm::uvec3> tVertIndex;
    std::vector<glm::uvec3> tTexIndex;
    std::vector<Chimera::VertexData> vl;
    std::vector<Chimera::Triangle> trisList;
};

#endif