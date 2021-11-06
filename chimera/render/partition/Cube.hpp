#pragma once

#include "chimera/render/partition/AABB.hpp"
#include "chimera/render/partition/Triangle.hpp"
#include <vector>

namespace Chimera {

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

enum class SPACE { EMPTY = 0, WALL = 1, DIAG = 2, FLOOR = 3, CEILING = 4, FC = 5, RAMP_FNS = 6, RAMP_FEW = 7, INVALID = 99 };

class Cube : public AABB {
  public:
    Cube(const char& caracter, const glm::vec3& min, const glm::vec3& max);
    virtual ~Cube();
    void setNeighbor(DEEP deep, CARDINAL card, Cube* pCube);
    void create(std::vector<VertexData>& vl, std::vector<Triangle>& tl);

  private:
    void newWall(std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    void newRamp(bool isFloor, CARDINAL card, std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    void newDiag(std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    void newFloor(std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    void newCeeling(std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    void newRampNSEW(SPACE space, std::vector<VertexData>& vl, std::vector<Triangle>& tl);

    inline SPACE getSpace() const { return this->space; }

    inline bool emptySpace() const {
        return ((this->space == SPACE::EMPTY) || (this->space == SPACE::FLOOR) || (this->space == SPACE::CEILING) ||
                (this->space == SPACE::FC));
    }

    CARDINAL emptyQuadrantDiag(DEEP deep, bool invert);
    bool hasNeighbor(DEEP deep, CARDINAL card, SPACE space);

    void newFlatFloorCeeling(bool isFloor, CARDINAL card, std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    void addFace(bool clockwise, int numFace, int numTex, std::vector<VertexData>& vl, std::vector<Triangle>& tl);
    Cube* pNorth;
    Cube* pEast;
    Cube* pSouth;
    Cube* pWest;
    Cube* pUp;
    Cube* pBottom;

    SPACE space;
    glm::vec2 t[4];
    std::vector<glm::uvec3> tVertIndex;
    std::vector<glm::uvec3> tTexIndex;
};
} // namespace Chimera
