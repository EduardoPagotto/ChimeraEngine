#pragma once
#include <glm/glm.hpp>

namespace ce {

    constexpr float epsilon{1e-2}; // FIXME: para 1e-1 Falhas em calculos de aproximação verificar projeto bsptree

    [[clang::always_inline]]
    inline const bool isNear(const float& v1, const float& v2) {
        return fabs(v1 - v2) < epsilon;
    }

    [[clang::always_inline]]
    inline const bool isNearV2(const glm::vec2& v1, const glm::vec2& v2) {
        return isNear(v1.x, v2.x) && isNear(v1.y, v2.y);
    }

    [[clang::always_inline]]
    inline const bool isNearV3(const glm::vec3& v1, const glm::vec3& v2) {
        return isNear(v1.x, v2.x) && isNear(v1.y, v2.y) && isNear(v1.z, v2.z);
    }

    [[clang::always_inline]]
    inline const glm::vec3 getSizeMinMax(const glm::vec3& min, const glm::vec3& max) {
        return glm::vec3(glm::abs(max.x - min.x), glm::abs(max.y - min.y), glm::abs(max.z - min.z));
    }

    [[clang::always_inline]]
    inline const bool isLessEpsilon(const float& val) {
        return fabs(val) < epsilon;
    }

    // inline glm::vec3 aproxEpsilon(const glm::vec3& dado) {
    //     return glm::vec3((fabs(dado.x) < epsilon) ? 0.0f : dado.x,  // X
    //                      (fabs(dado.y) < epsilon) ? 0.0f : dado.y,  // Y
    //                      (fabs(dado.z) < epsilon) ? 0.0f : dado.z); // Z
    // }

} // namespace ce
