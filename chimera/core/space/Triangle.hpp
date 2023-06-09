#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {
struct Triangle {
    Triangle(const Triangle& t) : point(t.point), normal(t.normal), splitter(t.splitter) {}
    Triangle(const glm::uvec3& t, const glm::vec3& _normal, const bool& split) : normal(_normal), splitter(split) { this->point = t; }
    bool splitter;
    glm::uvec3 point;
    glm::vec3 normal;
};

using TrisIndex = std::vector<glm::uvec3>;

} // namespace Chimera
