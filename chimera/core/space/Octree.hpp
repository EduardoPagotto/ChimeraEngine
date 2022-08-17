#pragma once
#include "AABB.hpp"
#include <vector>

namespace Chimera {

class Octree {
  public:
    Octree(const AABB& _boundary, const unsigned int& _capacity, Octree* _parent = nullptr, bool _leafMode = false, unsigned int _deep = 0);
    virtual ~Octree();

    void destroy();

    bool insert(const glm::vec3& _point);
    void insertAABB(const AABB& aabb);
    void query(const AABB& _aabb, std::vector<glm::vec3>& _found);
    bool hasPoint(const glm::vec3& point);

    inline Octree* getParent() { return pParent; }
    inline unsigned int getDeep() { return deep; }

    void getBondaryList(std::vector<AABB>& list, const bool& showEmpty);
    void dump_data(short indice);

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
    uint32_t serial;
    static uint32_t serial_master;
};
} // namespace Chimera