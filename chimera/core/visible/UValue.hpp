#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Chimera {

enum class UniformType {
    t_int,
    t_float,
    t_vec2,
    t_ivec2,
    t_vec3,
    t_ivec3,
    t_vec4,
    t_ivec4,
    t_mat3,
    t_mat4,
    tp_int,
    tp_float,
    tp_vec2,
    tp_ivec2,
    tp_vec3,
    tp_ivec3,
    tp_vec4,
    tp_ivec4,
    tp_mat3,
    tp_mat4,
    t_invalid
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
    float* p_float;
    glm::vec2* p_vec2;
    glm::ivec2* p_ivec2;
    glm::vec3* p_vec3;
    glm::ivec3* p_ivec3;
    glm::vec4* p_vec4;
    glm::ivec4* p_ivec4;
    glm::mat3* p_mat3;
    glm::mat4* p_mat4;
};

struct UValue {
    UValue(const UValue& o) : type(o.type), u(o.u), size(o.size) {}
    UValue(const int& _val) : type(UniformType::t_int), size(1) { u.v_int = _val; }
    UValue(const float& _val) : type(UniformType::t_float), size(1) { u.v_float = _val; }
    UValue(const glm::vec2& _val) : type(UniformType::t_vec2), size(1) { u.v_vec2 = _val; }
    UValue(const glm::ivec2& _val) : type(UniformType::t_ivec2), size(1) { u.v_ivec2 = _val; }
    UValue(const glm::vec3& _val) : type(UniformType::t_vec3), size(1) { u.v_vec3 = _val; }
    UValue(const glm::ivec3& _val) : type(UniformType::t_ivec3), size(1) { u.v_ivec3 = _val; }
    UValue(const glm::vec4& _val) : type(UniformType::t_vec4), size(1) { u.v_vec4 = _val; }
    UValue(const glm::ivec4& _val) : type(UniformType::t_ivec4), size(1) { u.v_ivec4 = _val; }
    UValue(const glm::mat3& _val) : type(UniformType::t_mat3), size(1) { u.v_mat3 = _val; }
    UValue(const glm::mat4& _val) : type(UniformType::t_mat4), size(1) { u.v_mat4 = _val; }
    UValue(const uint32_t& size, int* _val) : type(UniformType::tp_int), size(size) { u.p_int = _val; }
    UValue(const uint32_t& size, float* _val) : type(UniformType::tp_float), size(size) { u.p_float = _val; }
    UValue(const uint32_t& size, glm::vec2* _val) : type(UniformType::tp_vec2), size(size) { u.p_vec2 = _val; }
    UValue(const uint32_t& size, glm::ivec2* _val) : type(UniformType::tp_ivec2), size(size) { u.p_ivec2 = _val; }
    UValue(const uint32_t& size, glm::vec3* _val) : type(UniformType::tp_vec3), size(size) { u.p_vec3 = _val; }
    UValue(const uint32_t& size, glm::ivec3* _val) : type(UniformType::tp_ivec3), size(size) { u.p_ivec3 = _val; }
    UValue(const uint32_t& size, glm::vec4* _val) : type(UniformType::tp_vec4), size(size) { u.p_vec4 = _val; }
    UValue(const uint32_t& size, glm::ivec4* _val) : type(UniformType::tp_ivec4), size(size) { u.p_ivec4 = _val; }
    UValue(const uint32_t& size, glm::mat3* _val) : type(UniformType::tp_mat3), size(size) { u.p_mat3 = _val; }
    UValue(const uint32_t& size, glm::mat4* _val) : type(UniformType::tp_mat4), size(size) { u.p_mat4 = _val; }

    UniformType type;
    UniVariant u;
    uint32_t size;

    UValue() : type(UniformType::t_invalid) { u.v_int = 0; }
};
using MapUniform = std::unordered_map<std::string, UValue>;

} // namespace Chimera