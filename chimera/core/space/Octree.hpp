#pragma once
#include "AABB.hpp"
#include "chimera/core/HeapQ.hpp"
#include <queue>
#include <vector>

namespace Chimera {

class Octree {
  private:
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
    inline static uint32_t serial_master;

  public:
    Octree(const AABB& boundary, const uint32_t& capacity, Octree* parent, const bool& leafMode, const uint32_t& deep)
        : boundary(boundary), capacity(capacity), pParent(parent), leafMode(leafMode), deep(deep), divided(false), locked(false),
          serial(serial_master) {

        serial_master++;
        for (short i = 0; i < 8; i++)
            pChild[i] = nullptr;
    }

    virtual ~Octree() { destroy(); }

    void destroy() {
        if (divided == true) {
            divided = false;
            for (short i = 0; i < 8; i++) {
                pChild[i]->destroy();
                delete pChild[i];
                pChild[i] = nullptr;
            }
        }

        points.clear();
        indexes.clear();
    }

    bool insert(const glm::vec3& point, const uint32_t& index) {

        if (boundary.contains(point) == false)
            return false;

        bool a = (points.size() < capacity);
        bool b = (leafMode == false);
        bool c = (divided == false);

        if (a && (b || c)) {
            points.push_back(point);
            indexes.push_back(index);
            return true;
        }

        if (c)
            this->subdivide();

        if (!b) {
            for (uint32_t i = 0; i < points.size(); i++)
                this->insertNew(points[i], indexes[i]);

            points.clear();
            indexes.clear();
        }

        return this->insertNew(point, index);
    }

    void insertAABB(const AABB& aabb, const uint32_t& index) {
        const glm::vec3* v = aabb.getAllVertex();
        for (int i = 0; i < 8; i++)
            this->insert(v[i], index);

        this->insert(aabb.getPosition(), index);
    }

    void query(const AABB& aabb, std::vector<glm::vec3>& found) {

        if (boundary.intersects(aabb) == false)
            return;

        for (auto p : points) {
            if (aabb.contains(p) == true)
                found.push_back(p);
        }

        if (divided == true) {
            for (short i = 0; i < 8; i++)
                pChild[i]->query(aabb, found);
        }
    }

    bool hasPoint(const glm::vec3& point) {

        if (boundary.contains(point) == true) {
            if (divided == true) {
                for (short i = 0; i < 8; i++)
                    if (pChild[i]->hasPoint(point))
                        return true;
            }

            for (auto p : points) {
                if (isNearV3(p, point))
                    return true;
            }
        }
        return false;
    }

    void visible(const Frustum& frustum, std::queue<uint32_t>& qIndexes) {
        HeapQ<uint32_t> heapQ(false);
        this->_visible(frustum, heapQ);

        uint32_t last = -1;
        while (heapQ.empty() == false) {
            uint32_t n = heapQ.top();
            if (n != last) {
                qIndexes.push(n);
                last = n;
            }
            heapQ.pop();
        }
    }

    void getBondaryList(std::vector<AABB>& list, const bool& showEmpty) {

        if (divided == true) {
            for (short i = 0; i < 8; i++)
                pChild[i]->getBondaryList(list, showEmpty);
        } else {
            if ((points.size() > 0) || (showEmpty)) {
                list.push_back(boundary);
            }
        }
    }

    inline Octree* getParent() const { return pParent; }

    inline const uint32_t getDeep() const { return deep; }

    inline const bool isLocked() const { return locked; }

    inline void setLock(const bool& locked) { this->locked = locked; }

  private:
    void subdivide() {
        this->divided = true;

        const uint32_t newDeep = deep + 1;

        const glm::vec3 p = boundary.getPosition();
        const glm::vec3 s = boundary.getSize() / 2.0f;
        const glm::vec3 h = s / 2.0f;

        const float xmax = p.x + h.x;
        const float ymax = p.y + h.y;
        const float zmax = p.z + h.z;

        const float xmin = p.x - h.x;
        const float ymin = p.y - h.y;
        const float zmin = p.z - h.z;

        AABB bsw, bse, tsw, tse, bnw, bne, tnw, tne;

        bsw.setPosition(glm::vec3(xmin, ymin, zmin), s);
        bse.setPosition(glm::vec3(xmax, ymin, zmin), s);
        tsw.setPosition(glm::vec3(xmin, ymax, zmin), s);
        tse.setPosition(glm::vec3(xmax, ymax, zmin), s);
        bnw.setPosition(glm::vec3(xmin, ymin, zmax), s);
        bne.setPosition(glm::vec3(xmax, ymin, zmax), s);
        tnw.setPosition(glm::vec3(xmin, ymax, zmax), s);
        tne.setPosition(glm::vec3(xmax, ymax, zmax), s);

        pChild[(int)AabbBondery::BSW] = new Octree(bsw, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::BSE] = new Octree(bse, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::TSW] = new Octree(tsw, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::TSE] = new Octree(tse, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::BNW] = new Octree(bnw, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::BNE] = new Octree(bne, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::TNW] = new Octree(tnw, capacity, this, leafMode, newDeep);
        pChild[(int)AabbBondery::TNE] = new Octree(tne, capacity, this, leafMode, newDeep);
    }

    void _visible(const Frustum& frustum, HeapQ<uint32_t>& qIndexes) {

        if (boundary.visible(frustum)) {
            if (divided == true) {
                for (short i = 0; i < 8; i++)
                    pChild[i]->_visible(frustum, qIndexes);
            }

            uint32_t last = -1;
            for (auto& i : this->indexes) {
                if (i != last) {
                    qIndexes.push(i);
                    last = i;
                }
            }
        }
    }

    bool insertNew(const glm::vec3& point, const uint32_t& index) {
        for (short i = 0; i < 8; i++) {
            if (pChild[i]->insert(point, index))
                return true;
        }
        return false;
    }
};
} // namespace Chimera