#include "Cube.hpp"

// Cube::Cube(const glm::ivec3& pos, const glm::ivec3& size, const float& sizeBlock) {
Cube::Cube(const SPACE& space, const glm::ivec3& pos, const glm::vec3& min, const glm::vec3& max) {
    this->space = space;
    this->position = pos;
    this->min = min;
    this->max = max;

    this->p[0] = glm::vec3(min.x, min.y, min.z);
    this->p[1] = glm::vec3(max.x, min.y, min.z);
    this->p[2] = glm::vec3(max.x, max.y, min.z);
    this->p[3] = glm::vec3(min.x, max.y, min.z);
    this->p[4] = glm::vec3(min.x, min.y, max.z);
    this->p[5] = glm::vec3(max.x, min.y, max.z);
    this->p[6] = glm::vec3(max.x, max.y, max.z);
    this->p[7] = glm::vec3(min.x, max.y, max.z);

    this->t[0] = glm::vec2(0, 0);
    this->t[1] = glm::vec2(1, 0);
    this->t[2] = glm::vec2(1, 1);
    this->t[3] = glm::vec2(0, 1);

    this->tVertIndex.push_back(glm::uvec3(0, 1, 2)); // f0 N
    this->tVertIndex.push_back(glm::uvec3(2, 3, 0)); // f1 N
    this->tVertIndex.push_back(glm::uvec3(1, 5, 6)); // f2 E
    this->tVertIndex.push_back(glm::uvec3(6, 2, 1)); // f3 E
    this->tVertIndex.push_back(glm::uvec3(4, 5, 6)); // f4 S
    this->tVertIndex.push_back(glm::uvec3(6, 7, 4)); // f5 S
    this->tVertIndex.push_back(glm::uvec3(0, 4, 7)); // f6 W
    this->tVertIndex.push_back(glm::uvec3(7, 3, 0)); // f7 W
    this->tVertIndex.push_back(glm::uvec3(3, 2, 6)); // f8 UP
    this->tVertIndex.push_back(glm::uvec3(6, 7, 3)); // f9 UP
    this->tVertIndex.push_back(glm::uvec3(0, 1, 5)); // f10 DOWN
    this->tVertIndex.push_back(glm::uvec3(5, 4, 0)); // f11 DOWN
    this->tVertIndex.push_back(glm::uvec3(0, 5, 6)); // f12 DIA1
    this->tVertIndex.push_back(glm::uvec3(6, 3, 0)); // f13 DIA1
    this->tVertIndex.push_back(glm::uvec3(4, 1, 2)); // f14 DIA2
    this->tVertIndex.push_back(glm::uvec3(2, 7, 4)); // f15 DIA2
    this->tVertIndex.push_back(glm::uvec3(0, 1, 6)); // f16 RP NS
    this->tVertIndex.push_back(glm::uvec3(6, 7, 0)); // f17 RP NS
    this->tVertIndex.push_back(glm::uvec3(0, 2, 6)); // f18 RP EW
    this->tVertIndex.push_back(glm::uvec3(6, 4, 0)); // f19 RP EW
    this->tVertIndex.push_back(glm::uvec3(5, 4, 3)); // f20 RP SN
    this->tVertIndex.push_back(glm::uvec3(3, 2, 5)); // f21 RP SN
    this->tVertIndex.push_back(glm::uvec3(4, 0, 2)); // f22 RP WE
    this->tVertIndex.push_back(glm::uvec3(2, 6, 4)); // f23 RP WE

    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f0 N
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f1 N
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f2 E
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f3 E
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f4 S
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f5 S
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f6 W
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f7 W
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f8 UP
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f9 UP
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f10 DOWN
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f11 DOWN
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f12 DIA1
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f13 DIA1
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f14 DIA2
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f15 DIA2
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f16 RP NS
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f17 RP NS
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f18 RP EW
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f19 RP EW
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f20 PR SN
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f21 RP SN
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // f22 RP WE
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // f23 RP WE
}

Cube::~Cube() {}

void Cube::addFace(bool clockwise, int num) {

    glm::uvec3 tri = this->tVertIndex[num];
    glm::uvec3 tex = this->tTexIndex[num];

    glm::vec3 va = p[tri.x]; // PA
    glm::vec3 vb = p[tri.y]; // PB
    glm::vec3 vc = p[tri.z]; // PC

    glm::vec2 ta = t[tex.x]; // TA
    glm::vec2 tb = t[tex.y]; // TB
    glm::vec2 tc = t[tex.z]; // TC

    uint32_t ia, ib, ic;

    if (!clockwise) {
        ia = this->indexPointCount;
        ib = ia + 1;
        ic = ib + 1;
    } else {
        ic = this->indexPointCount;
        ib = ic + 1;
        ia = ib + 1;
    }

    this->vl.push_back({va, glm::vec3(0.0f), ta});
    this->vl.push_back({vb, glm::vec3(0.0f), tb});
    this->vl.push_back({vc, glm::vec3(0.0f), tc});

    Chimera::Triangle t1 = Chimera::Triangle(ia, ib, ic, glm::vec3(0.0f));
    t1.calcNormal(this->vl);
    this->trisList.push_back(t1);

    this->indexPointCount += 3;
}
