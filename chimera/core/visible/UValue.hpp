#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Chimera {

enum class UniformType {
    INVALID = 0,
    INT,
    INTP,
    IVEC2,
    IVEC3,
    IVEC4,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4,
};

union UniVariant {
    int vint;
    int* pint;
    glm::ivec2 vivec2;
    glm::ivec3 vivec3;
    glm::ivec4 vivec4;

    float vfloat;
    glm::vec2 vvec2;
    glm::vec3 vvec3;
    glm::vec4 vvec4;

    glm::mat3 vmat3;
    glm::mat4 vmat4;
};

struct UValue {
    UValue(const UValue& o) : type(o.type), u(o.u), size(o.size) {}

    UValue(const int& _val) : type(UniformType::INT), size(1) { u.vint = _val; }
    UValue(const uint32_t& size, int* _val) : type(UniformType::INTP), size(size) { u.pint = _val; }

    UValue(const glm::ivec2& _val) : type(UniformType::IVEC2), size(1) { u.vivec2 = _val; }
    UValue(const glm::ivec3& _val) : type(UniformType::IVEC3), size(1) { u.vivec3 = _val; }
    UValue(const glm::ivec4& _val) : type(UniformType::IVEC4), size(1) { u.vivec4 = _val; }

    UValue(const float& _val) : type(UniformType::FLOAT), size(1) { u.vfloat = _val; }

    UValue(const glm::vec2& _val) : type(UniformType::VEC2), size(1) { u.vvec2 = _val; }
    UValue(const glm::vec3& _val) : type(UniformType::VEC3), size(1) { u.vvec3 = _val; }
    UValue(const glm::vec4& _val) : type(UniformType::VEC4), size(1) { u.vvec4 = _val; }

    UValue(const glm::mat3& _val) : type(UniformType::MAT3), size(1) { u.vmat3 = _val; }
    UValue(const glm::mat4& _val) : type(UniformType::MAT4), size(1) { u.vmat4 = _val; }

    UniformType type;
    UniVariant u;

    uint32_t size;

    UValue() : type(UniformType::INVALID) { u.vint = 0; }
};
using MapUniform = std::unordered_map<std::string, UValue>;

} // namespace Chimera