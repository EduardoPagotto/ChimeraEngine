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
    int v_int;
    float v_float;
    glm::vec2 v_vec2;
    glm::ivec2 v_ivec2;
    glm::vec3 v_vec3;
    glm::ivec3 v_ivec3;
    glm::vec4 v_vec4;
    glm::ivec4 v_ivec4;
    glm::mat3 v_mat3;
    glm::mat4 v_mat4;
    int* p_int;
};

struct UValue {
    UValue(const UValue& o) : type(o.type), u(o.u), size(o.size) {}
    UValue(const int& _val) : type(UniformType::INT), size(1) { u.v_int = _val; }
    UValue(const float& _val) : type(UniformType::FLOAT), size(1) { u.v_float = _val; }
    UValue(const glm::vec2& _val) : type(UniformType::VEC2), size(1) { u.v_vec2 = _val; }
    UValue(const glm::ivec2& _val) : type(UniformType::IVEC2), size(1) { u.v_ivec2 = _val; }
    UValue(const glm::vec3& _val) : type(UniformType::VEC3), size(1) { u.v_vec3 = _val; }
    UValue(const glm::ivec3& _val) : type(UniformType::IVEC3), size(1) { u.v_ivec3 = _val; }
    UValue(const glm::vec4& _val) : type(UniformType::VEC4), size(1) { u.v_vec4 = _val; }
    UValue(const glm::ivec4& _val) : type(UniformType::IVEC4), size(1) { u.v_ivec4 = _val; }
    UValue(const glm::mat3& _val) : type(UniformType::MAT3), size(1) { u.v_mat3 = _val; }
    UValue(const glm::mat4& _val) : type(UniformType::MAT4), size(1) { u.v_mat4 = _val; }
    UValue(const uint32_t& size, int* _val) : type(UniformType::INTP), size(size) { u.p_int = _val; }

    UniformType type;
    UniVariant u;

    uint32_t size;

    UValue() : type(UniformType::INVALID) { u.v_int = 0; }
};
using MapUniform = std::unordered_map<std::string, UValue>;

} // namespace Chimera