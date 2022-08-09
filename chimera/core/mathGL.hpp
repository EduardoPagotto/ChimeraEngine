#pragma once
#include <glm/glm.hpp>

namespace Chimera {

#define EPSILON 1e-2 // FIXME: para 1e-1 Falhas em calculos de aproximação verificar projeto bsptree

bool is_near(const float& v1, const float& v2);
bool is_nearVec3(const glm::vec3& v1, const glm::vec3& v2);
bool is_nearVec2(const glm::vec2& v1, const glm::vec2& v2);
// glm::vec3 aproxEpsilon(const glm::vec3& dado);

} // namespace Chimera