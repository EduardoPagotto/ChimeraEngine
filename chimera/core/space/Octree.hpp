#pragma once
#include "AABB.hpp"
#include "chimera/core/HeapQ.hpp"
#include <memory>
#include <queue>
#include <vector>

namespace ce {

class Octree {
  private:
    [[maybe_unused]]
    Octree* pParent{nullptr};
    uint32_t capacity{27};
    bool leafMode{true};
    uint32_t deep{0};
    uint32_t serial{0};
    AABB boundary;
    std::vector<std::unique_ptr<Octree>> childs;
    std::vector<glm::vec3> points;
    std::vector<uint32_t> indexes;
    inline static uint32_t serial_master{0};

  public:
    explicit Octree(const glm::vec3& pos, const glm::vec3& size, Octree* parent) noexcept
        : pParent(parent), capacity(parent->capacity), leafMode(parent->leafMode), deep(parent->deep + 1),
          serial(serial_master++) {
        boundary.setPosition(pos, size);
    }

    explicit Octree(const AABB& boundary, const uint32_t& capacity, const bool& leafMode) noexcept
        : pParent(nullptr), capacity(capacity), leafMode(leafMode), deep(0), serial(serial_master++),
          boundary(boundary) {}

    virtual ~Octree() noexcept { destroy(); }

    void destroy() noexcept {
        if (!childs.empty()) {
            for (auto& octree : childs) {
                octree->destroy();
                octree = nullptr;
            }
            childs.clear();
        }

        points.clear();
        indexes.clear();
    }

    bool insert(const glm::vec3& point, const uint32_t& index) noexcept {

        if (boundary.contains(point) == false)
            return false;

        if ((points.size() < capacity) && ((!leafMode) || childs.empty())) {
            points.push_back(point);
            indexes.push_back(index);
            return true;
        }

        if (childs.empty())
            this->subdivide();

        if (leafMode) {
            for (std::size_t i = 0; i < points.size(); i++)
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

        for (auto& octree : childs) {
            octree->query(aabb, found);
        }
    }

    bool hasPoint(const glm::vec3& point) noexcept {

        if (boundary.contains(point) == true) {
            for (auto& octree : childs) {
                if (octree->hasPoint(point))
                    return true;
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

        if (!childs.empty()) {
            for (auto& octree : childs) {
                octree->getBondaryList(list, showEmpty);
            }
        } else {
            if ((points.size() > 0) || (showEmpty)) {
                list.push_back(boundary);
            }
        }
    }

  private:
    void subdivide() noexcept {

        const glm::vec3 s = boundary.getSize() / 2.0f;
        const glm::vec3 h = s / 2.0f;
        const glm::vec3 max = boundary.getPosition() + h;
        const glm::vec3 min = boundary.getPosition() - h;

        childs.push_back(std::make_unique<Octree>(glm::vec3(min.x, min.y, min.z), s, this)); // AabbBondery::BSW 0
        childs.push_back(std::make_unique<Octree>(glm::vec3(max.x, min.y, min.z), s, this)); // AabbBondery::BSE 1
        childs.push_back(std::make_unique<Octree>(glm::vec3(min.x, max.y, min.z), s, this)); // AabbBondery::TSW 2
        childs.push_back(std::make_unique<Octree>(glm::vec3(max.x, max.y, min.z), s, this)); // AabbBondery::TSE 3
        childs.push_back(std::make_unique<Octree>(glm::vec3(min.x, min.y, max.z), s, this)); // AabbBondery::BNW 4
        childs.push_back(std::make_unique<Octree>(glm::vec3(max.x, min.y, max.z), s, this)); // AabbBondery::BNE 5
        childs.push_back(std::make_unique<Octree>(glm::vec3(min.x, max.y, max.z), s, this)); // AabbBondery::TNW 6
        childs.push_back(std::make_unique<Octree>(glm::vec3(max.x, max.y, max.z), s, this)); // AabbBondery::TNE 7
    }

    void _visible(const Frustum& frustum, HeapQ<uint32_t>& qIndexes) noexcept {

        if (boundary.visible(frustum)) {
            for (auto& octree : childs) {
                octree->_visible(frustum, qIndexes);
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
        for (auto& octree : childs) {
            if (octree->insert(point, index))
                return true;
        }
        return false;
    }
};
} // namespace ce
