#include "Cube.hpp"

// Cube::Cube(const glm::ivec3& pos, const glm::ivec3& size, const float& sizeBlock) {
Cube::Cube(const char& caracter, const glm::ivec3& pos, const glm::vec3& min, const glm::vec3& max) {

    if (caracter == 0x20)
        this->space = SPACE::EMPTY;
    else
        this->space = (SPACE)(caracter - 0x30);

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
    this->tVertIndex.push_back(glm::uvec3(6, 7, 3)); // f9 UP    / C Q0(SW)
    this->tVertIndex.push_back(glm::uvec3(0, 1, 5)); // f10 DOWN /  F Q2(NE)
    this->tVertIndex.push_back(glm::uvec3(5, 4, 0)); // f11 DOWN /  F Q0(SW)

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
    this->tVertIndex.push_back(glm::uvec3(4, 0, 1)); // f24 F Q1(NW)
    this->tVertIndex.push_back(glm::uvec3(1, 5, 4)); // f25 F Q3(SE)
    this->tVertIndex.push_back(glm::uvec3(7, 3, 2)); // F26 C Q1(NW)
    this->tVertIndex.push_back(glm::uvec3(2, 6, 7)); // F27 C Q3(NW)

    //---
    this->tTexIndex.push_back(glm::uvec3(0, 1, 2)); // T0 Q0 /   N // FIXME: Trocar estrutura por indice de rotacao de textura!!!!
    this->tTexIndex.push_back(glm::uvec3(2, 3, 0)); // T1 Q2 /
    this->tTexIndex.push_back(glm::uvec3(1, 0, 3)); // T2 !Q0 /
    this->tTexIndex.push_back(glm::uvec3(3, 2, 1)); // T3 !Q2 /
    this->tTexIndex.push_back(glm::uvec3(0, 3, 2)); // T4 F Q1(NW) /
    this->tTexIndex.push_back(glm::uvec3(2, 1, 0)); // T5 F Q3(SE) /
    this->tTexIndex.push_back(glm::uvec3(3, 0, 1)); // T6 c Q1(NW)
    this->tTexIndex.push_back(glm::uvec3(1, 2, 3)); // T7 c
}

Cube::~Cube() {}

void Cube::addFace(bool clockwise, int numFace, int numTex, std::vector<Chimera::VertexData>& vl, std::vector<Chimera::Triangle>& tl) {

    glm::uvec3 tri = this->tVertIndex[numFace];
    glm::uvec3 tex = this->tTexIndex[numTex];

    glm::vec3 va = p[tri.x]; // PA
    glm::vec3 vb = p[tri.y]; // PB
    glm::vec3 vc = p[tri.z]; // PC

    glm::vec2 ta = t[tex.x]; // TA
    glm::vec2 tb = t[tex.y]; // TB
    glm::vec2 tc = t[tex.z]; // TC

    uint32_t ia, ib, ic;

    if (!clockwise) {
        ia = tl.size() * 3;
        ib = ia + 1;
        ic = ib + 1;
    } else {
        ic = tl.size() * 3;
        ib = ic + 1;
        ia = ib + 1;
    }

    vl.push_back({va, glm::vec3(0.0f), ta});
    vl.push_back({vb, glm::vec3(0.0f), tb});
    vl.push_back({vc, glm::vec3(0.0f), tc});

    Chimera::Triangle t1 = Chimera::Triangle(ia, ib, ic, glm::vec3(0.0f));
    t1.calcNormal(vl);
    tl.push_back(t1);
}
