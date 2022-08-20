#include "chimera/core/space/Octree.hpp"
#include "chimera/core/mathGL.hpp"
#include "chimera/src/core/HeapQ.cpp"
#include <SDL2/SDL.h>

namespace Chimera {

uint32_t Octree::serial_master = 0;

Octree::Octree(const AABB& boundary, const uint32_t& capacity, Octree* parent, const bool& leafMode, const uint32_t& deep)
    : boundary(boundary), capacity(capacity), pParent(parent), leafMode(leafMode), deep(deep), divided(false), locked(false),
      serial(serial_master) {

    serial_master++;

    for (short i = 0; i < 8; i++)
        pChild[i] = nullptr;
}

Octree::~Octree() { destroy(); }

void Octree::dump_data(short indice) {

    if (divided == true) {
        for (short i = 0; i < 8; i++) {
            pChild[i]->dump_data(i);
        }
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Serial: %d, Indice: %d Deep: %d, Points: %ld", this->serial, indice, this->deep,
                 this->points.size());
}

void Octree::destroy() {
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

void Octree::subdivide() {

    glm::vec3 p = boundary.getPosition();
    glm::vec3 s = boundary.getSize() / 2.0f;
    glm::vec3 h = s / 2.0f;
    unsigned int newDeep = deep + 1;
    divided = true;

    float xmax = p.x + h.x;
    float ymax = p.y + h.y;
    float zmax = p.z + h.z;

    float xmin = p.x - h.x;
    float ymin = p.y - h.y;
    float zmin = p.z - h.z;

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

bool Octree::insertNew(const glm::vec3& point, const uint32_t& index) {
    for (short i = 0; i < 8; i++) {
        if (pChild[i]->insert(point, index))
            return true;
    }
    return false;
}

void Octree::insertAABB(const AABB& aabb, const uint32_t& index) {
    const glm::vec3* v = aabb.getAllVertex();
    for (int i = 0; i < 8; i++)
        this->insert(v[i], index);

    this->insert(aabb.getPosition(), index);
}

bool Octree::insert(const glm::vec3& point, const uint32_t& index) {

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

bool Octree::hasPoint(const glm::vec3& point) {
    if (boundary.contains(point) == true) {
        if (divided == true) {
            for (short i = 0; i < 8; i++)
                if (pChild[i]->hasPoint(point))
                    return true;
        }

        for (auto p : points) {
            if (is_nearVec3(p, point))
                return true;
        }
    }
    return false;
}

void Octree::query(const AABB& aabb, std::vector<glm::vec3>& found) {

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

void Octree::getBondaryList(std::vector<AABB>& list, const bool& showEmpty) {

    if (divided == true) {
        for (short i = 0; i < 8; i++)
            pChild[i]->getBondaryList(list, showEmpty);
    } else {
        if ((points.size() > 0) || (showEmpty)) {
            list.push_back(boundary);
        }
    }
}

void Octree::visible(const Frustum& frustum, std::queue<uint32_t>& qIndexes) {

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

void Octree::_visible(const Frustum& frustum, HeapQ<uint32_t>& qIndexes) {
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
} // namespace Chimera
