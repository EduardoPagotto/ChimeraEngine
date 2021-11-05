#pragma once

#include "AABB.hpp"
#include <vector>

namespace Chimera {

enum class CHILDOCTREE {
    top_northwest = 0,
    top_northeast = 1,
    top_southwest = 2,
    top_southeast = 3,
    botton_northwest = 4,
    botton_northeast = 5,
    botton_southwest = 6,
    botton_southeast = 7,
};

class Octree {
  public:
    Octree(const AABB& _boundary, const unsigned int& _capacity, Octree* _parent = nullptr, bool _leafMode = false, unsigned int _deep = 0);
    virtual ~Octree();

    void destroy();

    bool insert(const glm::vec3& _point);
    void query(const AABB& _aabb, std::vector<glm::vec3>& _found);

    inline Octree* getParent() { return pParent; }
    inline unsigned int getDeep() { return deep; }

  private:
    void subdivide();
    bool insertNew(const glm::vec3& _point);

    bool leafMode;
    bool divided;
    unsigned int deep;
    unsigned int capacity;
    AABB boundary;
    Octree* pParent;
    Octree* pChild[8];
    std::vector<glm::vec3> points;
};
} // namespace Chimera