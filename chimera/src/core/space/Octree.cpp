#include "chimera/core/space/Octree.hpp"

namespace Chimera {

Octree::Octree(const AABB& _boundary, const unsigned int& _capacity, Octree* _parent, bool _leafMode, unsigned int _deep)
    : boundary(_boundary), capacity(_capacity), pParent(_parent), leafMode(_leafMode), deep(_deep) {

    for (short i = 0; i < 8; i++)
        pChild[i] = nullptr;

    divided = false;
}

Octree::~Octree() { destroy(); }

void Octree::destroy() {
    if (divided == true) {
        for (short i = 0; i < 8; i++) {
            pChild[i]->destroy();
            delete pChild[i];
            pChild[i] = nullptr;
        }
    }

    points.clear();
}

void Octree::subdivide() {

    glm::vec3 p = boundary.getPosition();
    glm::vec3 s = boundary.getSize() / 2.0f;
    unsigned int newDeep = deep + 1;
    divided = true;

    float xmax = p.x + s.x;
    float xmin = p.x - s.x;
    float ymax = p.y + s.y;
    float ymin = p.y - s.y;
    float zmax = p.z + s.z;
    float zmin = p.z - s.z;

    AABB tne, tnw, tsw, tse, bne, bnw, bsw, bse;

    tne.setPosition(glm::vec3(xmax, ymax, zmax), s);
    tnw.setPosition(glm::vec3(xmin, ymax, zmax), s);
    tsw.setPosition(glm::vec3(xmin, ymax, zmin), s);
    tse.setPosition(glm::vec3(xmax, ymax, zmin), s);

    bne.setPosition(glm::vec3(xmax, ymin, zmax), s);
    bnw.setPosition(glm::vec3(xmin, ymin, zmax), s);
    bsw.setPosition(glm::vec3(xmin, ymin, zmin), s);
    bse.setPosition(glm::vec3(xmax, ymin, zmin), s);

    pChild[(int)CHILDOCTREE::top_northeast] = new Octree(tne, capacity, this, leafMode, newDeep);
    pChild[(int)CHILDOCTREE::top_northwest] = new Octree(tnw, capacity, this, leafMode, newDeep);
    pChild[(int)CHILDOCTREE::top_southwest] = new Octree(tsw, capacity, this, leafMode, newDeep);
    pChild[(int)CHILDOCTREE::top_southeast] = new Octree(tse, capacity, this, leafMode, newDeep);

    pChild[(int)CHILDOCTREE::botton_northeast] = new Octree(bne, capacity, this, leafMode, newDeep);
    pChild[(int)CHILDOCTREE::botton_northwest] = new Octree(bnw, capacity, this, leafMode, newDeep);
    pChild[(int)CHILDOCTREE::botton_southwest] = new Octree(bsw, capacity, this, leafMode, newDeep);
    pChild[(int)CHILDOCTREE::botton_southeast] = new Octree(bse, capacity, this, leafMode, newDeep);
}

bool Octree::insertNew(const glm::vec3& _point) {
    for (short i = 0; i < 8; i++) {
        if (pChild[i]->insert(_point))
            return true;
    }
    return false;
}

bool Octree::insert(const glm::vec3& _point) {

    if (boundary.contains(_point) == false)
        return false;

    bool a = (points.size() < capacity);
    bool b = (leafMode == false);
    bool c = (divided == false);

    if ((a && b) || (a && c)) {
        points.push_back(_point);
        return true;
    }

    if (c)
        this->subdivide();

    if (!b) {
        for (glm::vec3 p : points)
            this->insertNew(p);

        points.clear();
    }

    return this->insertNew(_point);
}

void Octree::query(const AABB& _aabb, std::vector<glm::vec3>& _found) { // std::vector<glm::vec3> points

    if (boundary.intersects(_aabb) == false)
        return;

    for (auto p : points) {
        if (_aabb.contains(p) == true)
            _found.push_back(p);
    }

    if (divided == true) {
        for (short i = 0; i < 8; i++)
            pChild[i]->query(_aabb, _found);
    }
}
} // namespace Chimera
