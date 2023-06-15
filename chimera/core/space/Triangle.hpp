#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {
struct Triangle {
    Triangle(const Triangle& t) : idx(t.idx), normal(t.normal), splitter(t.splitter) {}
    Triangle(const glm::uvec3& t, const glm::vec3& _normal, const bool& split) : normal(_normal), splitter(split) { this->idx = t; }
    bool splitter;

    // index PA, PB, PC as S,T,P
    glm::uvec3 idx;
    glm::vec3 normal;
};

using TrisIndex = std::vector<glm::uvec3>;

} // namespace Chimera
