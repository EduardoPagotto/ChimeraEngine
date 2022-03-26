#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {
struct Mesh {
    Mesh() = default;
    bool singleIndex = false;
    std::vector<glm::vec3> point;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> uv;
    std::vector<uint32_t> iPoint;
    std::vector<uint32_t> iNormal;
    std::vector<uint32_t> iUv;
};

void meshDebug(Mesh* m, bool _showAll);

} // namespace Chimera