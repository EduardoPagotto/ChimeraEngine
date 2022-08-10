#include "chimera/core/space/Octree.hpp"
#include "chimera/core/mathGL.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

uint32_t Octree::serial_master = 0;

Octree::Octree(const AABB& _boundary, const unsigned int& _capacity, Octree* _parent, bool _leafMode, unsigned int _deep)
    : boundary(_boundary), capacity(_capacity), pParent(_parent), leafMode(_leafMode), deep(_deep) {

    serial = serial_master;
    serial_master++;

    for (short i = 0; i < 8; i++)
        pChild[i] = nullptr;

    divided = false;
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

    if (a && (b || c)) {
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

void Octree::query(const AABB& _aabb, std::vector<glm::vec3>& _found) {

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

void Octree::debug_render() {

    if (divided == true) {
        for (short i = 0; i < 8; i++)
            pChild[i]->debug_render();
    } else {

        boundary.debug_render();
    }
}

} // namespace Chimera
