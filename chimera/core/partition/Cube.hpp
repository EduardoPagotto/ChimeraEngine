#pragma once
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/space/Triangle.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include <vector>

namespace Chimera {

enum class CARDINAL {
    NORTH = 0,      //
    NORTH_EAST = 1, // RIGHT
    EAST = 2,       // RIGHT
    SOUTH_EAST = 3, // RIGHT
    SOUTH = 4,      //
    SOUTH_WEST = 5, // LEFT
    WEST = 6,       // LEFT
    NORTH_WEST = 7, // LEFT
    NONE = 8
};

enum class DEEP {
    UP = 0,     // ABOVE
    MIDDLE = 1, // LEVEL
    DOWN = 2    // UNDER
};

enum class SPACE { EMPTY = 0, SOLID = 1, DIAG = 2, FLOOR = 3, CEILING = 4, FC = 5, RAMP_FNS = 6, RAMP_FEW = 7, INVALID = 99 };

class Cube : public AABB {
  public:
    Cube(const char& caracter, const glm::vec3& min, const glm::vec3& max);
    virtual ~Cube();
    void setNeighbor(DEEP deep, CARDINAL card, Cube* pCube);
    void create(Mesh* mesh);

  private:
    void newWall();
    void newRamp(bool isFloor, CARDINAL card);
    void newDiag();
    void newFloor();
    void newCeeling();
    void newRampNSEW(SPACE space);

    inline SPACE getSpace() const { return this->space; }

    inline bool emptySpace() const {
        return ((this->space == SPACE::EMPTY) || (this->space == SPACE::FLOOR) || (this->space == SPACE::CEILING) ||
                (this->space == SPACE::FC));
    }

    CARDINAL emptyQuadrantDiag(DEEP deep, bool invert);
    bool hasNeighbor(DEEP deep, CARDINAL card, SPACE space);

    void newFlatFloorCeeling(bool isFloor, CARDINAL card);
    void addFace(bool clockwise, int numFace, int numTex);
    Cube* pNorth;
    Cube* pEast;
    Cube* pSouth;
    Cube* pWest;
    Cube* pUp;
    Cube* pDown;
    SPACE space;
    Mesh* mesh;
};

void initCubeBase();
void cleanupCubeBase();
glm::ivec3 getCardinalPos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos);
glm::vec3 minimal(const float& sizeBlock, const glm::vec3 halfBlock, const glm::ivec3& pos);
uint32_t getIndexArrayPos(const glm::ivec3& pos, const glm::ivec3& size);
Cube* getCubeNeighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos, const glm::ivec3& size, std::vector<Cube*>& vpCube);
const glm::ivec3 createMazeCube(const char filename[], float sizeBlock, std::vector<Cube*>& vpCube);
void linkCubes(const glm::ivec3& size, std::vector<Cube*>& vpCube);
} // namespace Chimera
