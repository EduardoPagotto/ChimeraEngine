#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {
struct Triangle {
    bool splitter{false};
    glm::uvec3 idx{0};
    glm::vec3 normal{0.0};

    Triangle(const Triangle& t) : idx(t.idx), normal(t.normal), splitter(t.splitter) {}
    Triangle(const glm::uvec3& t, const glm::vec3& _normal, const bool& split) : normal(_normal), splitter(split) { this->idx = t; }
};

using TrisIndex = std::vector<glm::uvec3>;

} // namespace Chimera
