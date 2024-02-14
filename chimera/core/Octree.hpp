#pragma once
#include "AABB.hpp"
#include "HeapQ.hpp"
#include <queue>
#include <vector>

namespace Chimera {

class Octree {
  private:
    bool m_locked, m_leafMode, m_divided;
    uint32_t m_deep, m_capacity, m_serial;
    AABB m_boundary;
    Octree* m_parent;
    Octree* m_childs[8];
    std::vector<glm::vec3> m_points;
    std::vector<uint32_t> m_indexes;
    inline static uint32_t s_serial_master;

  public:
    Octree(const AABB& boundary, const uint32_t& capacity, Octree* parent, const bool& leafMode, const uint32_t& deep)
        : m_boundary(boundary), m_capacity(capacity), m_parent(parent), m_leafMode(leafMode), m_deep(deep), m_divided(false),
          m_locked(false), m_serial(s_serial_master) {

        s_serial_master++;

        for (short i = 0; i < 8; i++)
            m_childs[i] = nullptr;
    }

    virtual ~Octree() { destroy(); }

    void destroy() {

        if (m_divided == true) {
            m_divided = false;
            for (short i = 0; i < 8; i++) {
                m_childs[i]->destroy();
                delete m_childs[i];
                m_childs[i] = nullptr;
            }
        }

        m_points.clear();
        m_indexes.clear();
    }

    bool insert(const glm::vec3& point, const uint32_t& index) {

        if (m_boundary.contains(point) == false)
            return false;

        bool a = (m_points.size() < m_capacity);
        bool b = (m_leafMode == false);
        bool c = (m_divided == false);

        if (a && (b || c)) {
            m_points.push_back(point);
            m_indexes.push_back(index);
            return true;
        }

        if (c)
            this->subdivide();

        if (!b) {
            for (uint32_t i = 0; i < m_points.size(); i++)
                this->insertNew(m_points[i], m_indexes[i]);

            m_points.clear();
            m_indexes.clear();
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

        if (m_boundary.intersects(aabb) == false)
            return;

        for (auto p : m_points) {
            if (aabb.contains(p) == true)
                found.push_back(p);
        }

        if (m_divided == true) {
            for (short i = 0; i < 8; i++)
                m_childs[i]->query(aabb, found);
        }
    }

    bool hasPoint(const glm::vec3& point) {

        if (m_boundary.contains(point) == true) {
            if (m_divided == true) {
                for (short i = 0; i < 8; i++)
                    if (m_childs[i]->hasPoint(point))
                        return true;
            }

            for (auto p : m_points) {
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

        if (m_divided == true) {
            for (short i = 0; i < 8; i++)
                m_childs[i]->getBondaryList(list, showEmpty);
        } else {
            if ((m_points.size() > 0) || (showEmpty)) {
                list.push_back(m_boundary);
            }
        }
    }

    // void dump_data(short indice) {
    //     if (m_divided == true) {
    //         for (short i = 0; i < 8; i++) {
    //             m_childs[i]->dump_data(i);
    //         }
    //     }

    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Serial: %d, Indice: %d Deep: %d, Points: %ld", this->m_serial, indice, this->m_deep,
    //                  this->m_points.size());
    // }

    inline Octree* getParent() const { return m_parent; }

    inline const uint32_t getDeep() const { return m_deep; }

    inline const bool isLocked() const { return m_locked; }

    inline void setLock(const bool& locked) { this->m_locked = locked; }

  private:
    void subdivide() {

        const glm::vec3 p{m_boundary.getPosition()};
        const glm::vec3 s{m_boundary.getSize() / 2.0f};
        const glm::vec3 h{s / 2.0f};

        const unsigned int newDeep = m_deep + 1;

        const float xmax = p.x + h.x;
        const float ymax = p.y + h.y;
        const float zmax = p.z + h.z;

        const float xmin = p.x - h.x;
        const float ymin = p.y - h.y;
        const float zmin = p.z - h.z;

        m_divided = true;

        AABB bsw, bse, tsw, tse, bnw, bne, tnw, tne;

        bsw.setPosition(glm::vec3(xmin, ymin, zmin), s);
        bse.setPosition(glm::vec3(xmax, ymin, zmin), s);
        tsw.setPosition(glm::vec3(xmin, ymax, zmin), s);
        tse.setPosition(glm::vec3(xmax, ymax, zmin), s);
        bnw.setPosition(glm::vec3(xmin, ymin, zmax), s);
        bne.setPosition(glm::vec3(xmax, ymin, zmax), s);
        tnw.setPosition(glm::vec3(xmin, ymax, zmax), s);
        tne.setPosition(glm::vec3(xmax, ymax, zmax), s);

        m_childs[(int)AabbBondery::BSW] = new Octree(bsw, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::BSE] = new Octree(bse, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::TSW] = new Octree(tsw, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::TSE] = new Octree(tse, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::BNW] = new Octree(bnw, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::BNE] = new Octree(bne, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::TNW] = new Octree(tnw, m_capacity, this, m_leafMode, newDeep);
        m_childs[(int)AabbBondery::TNE] = new Octree(tne, m_capacity, this, m_leafMode, newDeep);
    }

    void _visible(const Frustum& frustum, HeapQ<uint32_t>& qIndexes) {

        if (m_boundary.visible(frustum)) {
            if (m_divided == true) {
                for (short i = 0; i < 8; i++)
                    m_childs[i]->_visible(frustum, qIndexes);
            }

            uint32_t last = -1;
            for (auto& i : this->m_indexes) {
                if (i != last) {
                    qIndexes.push(i);
                    last = i;
                }
            }
        }
    }

    bool insertNew(const glm::vec3& point, const uint32_t& index) {
        for (short i = 0; i < 8; i++) {
            if (m_childs[i]->insert(point, index))
                return true;
        }
        return false;
    }
};
} // namespace Chimera