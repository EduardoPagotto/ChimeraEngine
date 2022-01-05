#pragma once
#include <glm/glm.hpp>

namespace Chimera {

#define TRI_PA 0
#define TRI_PB 1
#define TRI_PC 2

struct Triangle {
    Triangle(const Triangle& t) : normal(t.normal), splitter(t.splitter) {
        this->p[TRI_PA] = t.p[TRI_PA];
        this->p[TRI_PB] = t.p[TRI_PB];
        this->p[TRI_PC] = t.p[TRI_PC];
    }

    Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const glm::vec3& _normal, const bool& split)
        : normal(_normal), splitter(split) {
        this->p[TRI_PA] = _pa;
        this->p[TRI_PB] = _pb;
        this->p[TRI_PC] = _pc;
    }

    uint32_t p[3]; // TRI_PA = 0, TRI_PB = 1, TRI_PC = 3
    bool splitter;
    glm::vec3 normal;
};
} // namespace Chimera
