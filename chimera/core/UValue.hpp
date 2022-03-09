#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Chimera {

enum class UniformType { t_int, t_float, t_vec2, t_ivec2, t_vec3, t_ivec3, t_vec4, t_ivec4, t_mat3, t_mat4, t_invalid };

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
};

struct UValue {
    UValue(const UValue& o) : type(o.type), u(o.u) {}
    UValue(const int& _val) : type(UniformType::t_int) { u.v_int = _val; }
    UValue(const float& _val) : type(UniformType::t_float) { u.v_float = _val; }
    UValue(const glm::vec2& _val) : type(UniformType::t_vec2) { u.v_vec2 = _val; }
    UValue(const glm::ivec2& _val) : type(UniformType::t_ivec2) { u.v_ivec2 = _val; }
    UValue(const glm::vec3& _val) : type(UniformType::t_vec3) { u.v_vec3 = _val; }
    UValue(const glm::ivec3& _val) : type(UniformType::t_ivec3) { u.v_ivec3 = _val; }
    UValue(const glm::vec4 _val) : type(UniformType::t_vec4) { u.v_vec4 = _val; }
    UValue(const glm::ivec4 _val) : type(UniformType::t_ivec4) { u.v_ivec4 = _val; }
    UValue(const glm::mat3& _val) : type(UniformType::t_mat3) { u.v_mat3 = _val; }
    UValue(const glm::mat4& _val) : type(UniformType::t_mat4) { u.v_mat4 = _val; }
    UniformType type;
    UniVariant u;

    UValue() : type(UniformType::t_invalid) { u.v_int = 0; }
};

using MapUniform = std::unordered_map<std::string, UValue>;

} // namespace Chimera