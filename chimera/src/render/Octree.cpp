#include "chimera/render/Octree.hpp"

namespace Chimera {

Octree::Octree(const AABB& _boundary, const unsigned int& _capacity) : boundary(boundary), capacity(_capacity) {

    top_northwest = nullptr;
    top_northeast = nullptr;
    top_southwest = nullptr;
    top_southeast = nullptr;

    botton_northwest = nullptr;
    botton_northeast = nullptr;
    botton_southwest = nullptr;
    botton_southeast = nullptr;

    divided = false;
}

Octree::~Octree() { destroy(); }

void Octree::destroy() {
    if (divided == true) {
        top_northwest->destroy();
        top_northeast->destroy();
        top_southwest->destroy();
        top_southeast->destroy();
        botton_northwest->destroy();
        botton_northeast->destroy();
        botton_southwest->destroy();
        botton_southeast->destroy();

        delete top_northwest;
        delete top_northeast;
        delete top_southwest;
        delete top_southeast;
        delete botton_northwest;
        delete botton_northeast;
        delete botton_southwest;
        delete botton_southeast;

        top_northwest = nullptr;
        top_northeast = nullptr;
        top_southwest = nullptr;
        top_southeast = nullptr;
        botton_northwest = nullptr;
        botton_northeast = nullptr;
        botton_southwest = nullptr;
        botton_southeast = nullptr;
    }

    points.clear();
}

void Octree::subdivide() {

    glm::vec3 p = boundary.getPosition();
    glm::vec3 s = boundary.getSize() / 2.0f;

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

    top_northeast = new Octree(tne, capacity);
    top_northwest = new Octree(tnw, capacity);
    top_southwest = new Octree(tsw, capacity);
    top_southeast = new Octree(tse, capacity);

    botton_northeast = new Octree(bne, capacity);
    botton_northwest = new Octree(bnw, capacity);
    botton_southwest = new Octree(bsw, capacity);
    botton_southeast = new Octree(bse, capacity);

    divided = true;
}

bool Octree::insert(const glm::vec3& _point) {

    if (boundary.contains(_point) == false)
        return false;

    if (points.size() < capacity) {
        points.push_back(_point);
        return true;
    }

    if (divided == false)
        subdivide();

    if (top_northeast->insert(_point))
        return true;

    if (top_northwest->insert(_point))
        return true;

    if (top_southeast->insert(_point))
        return true;

    if (top_southwest->insert(_point))
        return true;

    if (botton_northeast->insert(_point))
        return true;

    if (botton_northwest->insert(_point))
        return true;

    if (botton_southeast->insert(_point))
        return true;

    if (botton_southwest->insert(_point))
        return true;

    return false;
}

void Octree::query(const AABB& _aabb, std::vector<glm::vec3>& _found) { // std::vector<glm::vec3> points

    // if not _found:
    //     _found = []

    if (boundary.intersects(_aabb) == false)
        return;
    else {
        for (auto p : points) {
            if (_aabb.contains(p) == true)
                _found.push_back(p);
        }

        if (divided == true) {
            top_northwest->query(_aabb, _found);
            top_northeast->query(_aabb, _found);
            top_southwest->query(_aabb, _found);
            top_southeast->query(_aabb, _found);
            botton_northwest->query(_aabb, _found);
            botton_northeast->query(_aabb, _found);
            botton_southwest->query(_aabb, _found);
            botton_southeast->query(_aabb, _found);
        }
    }
    // return _found;
}
} // namespace Chimera
