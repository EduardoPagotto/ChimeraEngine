#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Chimera {

enum class UniformType {
    INVALID = 0,
    INT,
    // UINT,
    INT_VEC,
    IVEC2,
    IVEC3,
    IVEC4,
    FLOAT,
    FLOAT_VEC,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4,
};

struct UValue {
    UValue() : type(UniformType::INVALID) {}
    UValue(const UValue& o) : type(o.type), ptr(o.ptr) {}
    virtual ~UValue() = default;

    void operator=(const UValue& o) {
        type = o.type;
        ptr = o.ptr;
    }

    UValue(const std::vector<int>& val) : type(UniformType::INT_VEC) {
        ptr = std::static_pointer_cast<void>(std::make_shared<std::vector<int>>(val));
    }
    UValue(const int& val) : type(UniformType::INT) { ptr = std::static_pointer_cast<void>(std::make_shared<int>(val)); }
    UValue(const glm::ivec2& val) : type(UniformType::IVEC2) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::ivec2>(val)); }
    UValue(const glm::ivec3& val) : type(UniformType::IVEC3) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::ivec3>(val)); }
    UValue(const glm::ivec4& val) : type(UniformType::IVEC4) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::ivec4>(val)); }

    UValue(const std::vector<float>& val) : type(UniformType::FLOAT_VEC) {
        ptr = std::static_pointer_cast<void>(std::make_shared<std::vector<float>>(val));
    }
    UValue(const float& val) : type(UniformType::FLOAT) { ptr = std::static_pointer_cast<void>(std::make_shared<float>(val)); }
    UValue(const glm::vec2& val) : type(UniformType::VEC2) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::vec2>(val)); }
    UValue(const glm::vec3& val) : type(UniformType::VEC3) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::vec3>(val)); }
    UValue(const glm::vec4& val) : type(UniformType::VEC4) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::vec4>(val)); }
    UValue(const glm::mat3& val) : type(UniformType::MAT3) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::mat3>(val)); }
    UValue(const glm::mat4& val) : type(UniformType::MAT4) { ptr = std::static_pointer_cast<void>(std::make_shared<glm::mat4>(val)); }

    UniformType type;
    std::shared_ptr<void> ptr;
};
using MapUniform = std::unordered_map<std::string, UValue>;

} // namespace Chimera