#pragma once
#include "AABB.hpp"
#include "chimera/core/HeapQ.hpp"
#include <memory>
#include <queue>
#include <vector>

namespace Chimera {

class Octree {
  private:
    bool leafMode{true};
    bool divided{false};
    uint32_t deep{0};
    uint32_t capacity{27};
    uint32_t serial{0};
    AABB boundary;
    Octree* pParent{nullptr};
    std::vector<std::unique_ptr<Octree>> child;
    std::vector<glm::vec3> points;
    std::vector<uint32_t> indexes;
    inline static uint32_t serial_master{0};

  public:
    explicit Octree(const AABB& boundary, Octree* parent) noexcept
        : boundary(boundary), pParent(parent), capacity(parent->capacity), leafMode(parent->leafMode), deep(parent->deep + 1),
          divided(false), serial(serial_master++) {}

    explicit Octree(const AABB& boundary, const uint32_t& capacity, const bool& leafMode) noexcept
        : boundary(boundary), capacity(capacity), leafMode(leafMode), pParent(nullptr), deep(0), divided(false), serial(serial_master++) {}

    virtual ~Octree() noexcept { destroy(); }

    void destroy() noexcept {
        if (divided == true) {
            divided = false;
            for (auto& octree : child) {
                octree->destroy();
                octree = nullptr;
            }
        }

        points.clear();
        indexes.clear();
    }

    bool insert(const glm::vec3& point, const uint32_t& index) noexcept {

        if (boundary.contains(point) == false)
            return false;

        const bool a = (points.size() < capacity);
        const bool b = (leafMode == false);
        const bool c = (divided == false);

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

    void insertAABB(const AABB& aabb, const uint32_t& index) noexcept {
        const std::vector<glm::vec3>& vList = aabb.getAllVertex();
        for (const glm::vec3& p : vList) {
            this->insert(p, index);
        }

        this->insert(aabb.getPosition(), index);
    }

    void query(const AABB& aabb, std::vector<glm::vec3>& found) noexcept {

        if (boundary.intersects(aabb) == false)
            return;

        for (auto p : points) {
            if (aabb.contains(p) == true)
                found.push_back(p);
        }

        if (divided == true) {
            for (auto& octree : child) {
                octree->query(aabb, found);
            }
        }
    }

    bool hasPoint(const glm::vec3& point) noexcept {

        if (boundary.contains(point) == true) {
            if (divided == true) {
                for (auto& octree : child) {
                    if (octree->hasPoint(point))
                        return true;
                }
            }

            for (auto p : points) {
                if (isNearV3(p, point))
                    return true;
            }
        }
        return false;
    }

    void visible(const Frustum& frustum, std::queue<uint32_t>& qIndexes) noexcept {
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

    void getBondaryList(std::vector<AABB>& list, const bool& showEmpty) noexcept {

        if (divided == true) {
            for (auto& octree : child) {
                octree->getBondaryList(list, showEmpty);
            }
        } else {
            if ((points.size() > 0) || (showEmpty)) {
                list.push_back(boundary);
            }
        }
    }

    // inline Octree* getParent() const { return pParent; }
    // inline const uint32_t getDeep() const { return deep; }

  private:
    void subdivide() noexcept {
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

        child.push_back(std::make_unique<Octree>(bsw, this)); // AabbBondery::BSW 0
        child.push_back(std::make_unique<Octree>(bse, this)); // AabbBondery::BSE 1
        child.push_back(std::make_unique<Octree>(tsw, this)); // AabbBondery::TSW 2
        child.push_back(std::make_unique<Octree>(tse, this)); // AabbBondery::TSE 3
        child.push_back(std::make_unique<Octree>(bnw, this)); // AabbBondery::BNW 4
        child.push_back(std::make_unique<Octree>(bne, this)); // AabbBondery::BNE 5
        child.push_back(std::make_unique<Octree>(tnw, this)); // AabbBondery::TNW 6
        child.push_back(std::make_unique<Octree>(tne, this)); // AabbBondery::TNE 7
    }

    void _visible(const Frustum& frustum, HeapQ<uint32_t>& qIndexes) noexcept {

        if (boundary.visible(frustum)) {
            if (divided == true) {
                for (auto& octree : child) {
                    octree->_visible(frustum, qIndexes);
                }
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

    bool insertNew(const glm::vec3& point, const uint32_t& index) noexcept {
        for (auto& octree : child) {
            if (octree->insert(point, index))
                return true;
        }
        return false;
    }
};
} // namespace Chimera