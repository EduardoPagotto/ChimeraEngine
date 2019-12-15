#ifndef __CHIMERA_OCTREE_HPP
#define __CHIMERA_OCTREE_HPP

#include "chimera/render/AABB.hpp"
#include <vector>

namespace Chimera {

class Octree {
  public:
    Octree(const AABB& _boundary, const unsigned int& _capacity);
    virtual ~Octree();

    void destroy();

    bool insert(const glm::vec3& _point);
    void query(const AABB& _aabb, std::vector<glm::vec3>& _found);

  private:
    void subdivide();

    unsigned int capacity;
    bool divided;
    AABB boundary;

    Octree* top_northwest;
    Octree* top_northeast;
    Octree* top_southwest;
    Octree* top_southeast;

    Octree* botton_northwest;
    Octree* botton_northeast;
    Octree* botton_southwest;
    Octree* botton_southeast;

    std::vector<glm::vec3> points;
};

} // namespace Chimera

// namespace Chimera
#endif