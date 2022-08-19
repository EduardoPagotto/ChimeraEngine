#pragma once
#include "AABB.hpp"
#include <queue>
#include <vector>

namespace Chimera {

class Octree {
  public:
    Octree(const AABB& _boundary, const unsigned int& _capacity, Octree* _parent = nullptr, bool _leafMode = false, unsigned int _deep = 0);
    virtual ~Octree();

    void destroy();

    bool insert(const glm::vec3& _point, const uint32_t& _index);
    void insertAABB(const AABB& aabb, const uint32_t& _index);
    void query(const AABB& _aabb, std::vector<glm::vec3>& _found);
    bool hasPoint(const glm::vec3& point);
    void visible(const Frustum& frustum, std::queue<uint32_t>& qIndexes);

    inline Octree* getParent() { return pParent; }
    inline unsigned int getDeep() { return deep; }

    void getBondaryList(std::vector<AABB>& list, const bool& showEmpty);
    void dump_data(short indice);

  private:
    void subdivide();
    bool insertNew(const glm::vec3& _point, const uint32_t& _index);

    bool leafMode;
    bool divided;
    unsigned int deep;
    unsigned int capacity;
    AABB boundary;
    Octree* pParent;
    Octree* pChild[8];
    std::vector<glm::vec3> points;
    std::vector<uint32_t> indexes;
    uint32_t serial;
    static uint32_t serial_master;
};
} // namespace Chimera