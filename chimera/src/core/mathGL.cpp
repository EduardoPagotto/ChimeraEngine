#include "chimera/core/mathGL.hpp"

namespace Chimera {
bool is_near(const float& v1, const float& v2) { return fabs(v1 - v2) < EPSILON; } // 0.01f
bool is_nearVec3(const glm::vec3& v1, const glm::vec3& v2) { return is_near(v1.x, v2.x) && is_near(v1.y, v2.y) && is_near(v1.z, v2.z); }
bool is_nearVec2(const glm::vec2& v1, const glm::vec2& v2) { return is_near(v1.x, v2.x) && is_near(v1.y, v2.y); }
// glm::vec3 aproxEpsilon(const glm::vec3& dado) {
//     return glm::vec3((fabs(dado.x) < EPSILON) ? 0.0f : dado.x,  // X
//                      (fabs(dado.y) < EPSILON) ? 0.0f : dado.y,  // Y
//                      (fabs(dado.z) < EPSILON) ? 0.0f : dado.z); // Z
// }
} // namespace Chimera