#pragma once
#include "AABB.hpp"
#include "chimera/core/HeapQ.hpp"
#include <queue>
#include <vector>

namespace Chimera {

class Octree {
  public:
    Octree(const AABB& boundary, const uint32_t& capacity, Octree* parent, const bool& leafMode, const uint32_t& deep);
    virtual ~Octree();

    void destroy();

    bool insert(const glm::vec3& point, const uint32_t& index);
    void insertAABB(const AABB& aabb, const uint32_t& index);
    void query(const AABB& aabb, std::vector<glm::vec3>& found);
    bool hasPoint(const glm::vec3& point);
    void visible(const Frustum& frustum, std::queue<uint32_t>& qIndexes);
    void getBondaryList(std::vector<AABB>& list, const bool& showEmpty);
    void dump_data(short indice);

    inline Octree* getParent() const { return pParent; }
    inline const uint32_t getDeep() const { return deep; }
    inline const bool isLocked() const { return locked; }
    inline void setLock(const bool& locked) { this->locked = locked; }

  private:
    void subdivide();
    void _visible(const Frustum& frustum, HeapQ<uint32_t>& qIndexes);
    bool insertNew(const glm::vec3& point, const uint32_t& index);

    bool locked;
    bool leafMode;
    bool divided;
    uint32_t deep;
    uint32_t capacity;
    uint32_t serial;
    AABB boundary;
    Octree* pParent;
    Octree* pChild[8];
    std::vector<glm::vec3> points;
    std::vector<uint32_t> indexes;
    static uint32_t serial_master;
};
} // namespace Chimera