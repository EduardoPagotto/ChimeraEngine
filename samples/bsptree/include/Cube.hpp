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
    Cube(const char& caracter, const glm::ivec3& pos, const glm::vec3& min, const glm::vec3& max);
    virtual ~Cube();

    void newWall(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);
    void newRamp(bool isFloor, CARDINAL card, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);
    void newDiag(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);
    void newFloor(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);
    void newCeeling(std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);

    inline SPACE getSpace() const { return this->space; }

    void setNeighbor(DEEP deep, CARDINAL card, Cube* pCube);
    inline bool emptySpace() const {
        return ((this->space == SPACE::EMPTY) || (this->space == SPACE::FLOOR) || (this->space == SPACE::CEILING) ||
                (this->space == SPACE::FC));
    }

    CARDINAL emptyQuadrantDiag(DEEP deep, bool invert);
    bool hasNeighbor(DEEP deep, CARDINAL card, SPACE space);

  private:
    void newFlatFloorCeeling(bool isFloor, CARDINAL card, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);
    void addFace(bool clockwise, int numFace, int numTex, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl);
    Cube* pNorth;
    Cube* pEast;
    Cube* pSouth;
    Cube* pWest;
    Cube* pUp;
    Cube* pBottom;

    SPACE space;
    glm::ivec3 position;
    glm::vec3 p[8];
    glm::vec2 t[4];
    glm::vec3 max;
    glm::vec3 min;
    std::vector<glm::uvec3> tVertIndex;
    std::vector<glm::uvec3> tTexIndex;
};

#endif