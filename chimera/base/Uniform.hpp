#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ce {

    enum class UniformType : uint8_t {
        INVALID = 0,
        INT,
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
        MAT4
    };

    struct Uniform {
        UniformType type{UniformType::INVALID};
        std::shared_ptr<void> ptr{nullptr};

        Uniform() = default;

        Uniform(const Uniform& o) : type(o.type), ptr(o.ptr) {}

        virtual ~Uniform() = default;

        void operator=(const Uniform& o) {
            type = o.type;
            ptr = o.ptr;
        }

        Uniform(const std::vector<int>& val)
            : type(UniformType::INT_VEC), ptr(std::static_pointer_cast<void>(std::make_shared<std::vector<int>>(val))) {
        }

        Uniform(const int& val)
            : type(UniformType::INT), ptr(std::static_pointer_cast<void>(std::make_shared<int>(val))) {}

        Uniform(const glm::ivec2& val)
            : type(UniformType::IVEC2), ptr(std::static_pointer_cast<void>(std::make_shared<glm::ivec2>(val))) {}

        Uniform(const glm::ivec3& val)
            : type(UniformType::IVEC3), ptr(std::static_pointer_cast<void>(std::make_shared<glm::ivec3>(val))) {}

        Uniform(const glm::ivec4& val)
            : type(UniformType::IVEC4), ptr(std::static_pointer_cast<void>(std::make_shared<glm::ivec4>(val))) {}

        Uniform(const std::vector<float>& val)
            : type(UniformType::FLOAT_VEC),
              ptr(std::static_pointer_cast<void>(std::make_shared<std::vector<float>>(val))) {}

        Uniform(const float& val)
            : type(UniformType::FLOAT), ptr(std::static_pointer_cast<void>(std::make_shared<float>(val))) {}

        Uniform(const glm::vec2& val)
            : type(UniformType::VEC2), ptr(std::static_pointer_cast<void>(std::make_shared<glm::vec2>(val))) {}

        Uniform(const glm::vec3& val)
            : type(UniformType::VEC3), ptr(std::static_pointer_cast<void>(std::make_shared<glm::vec3>(val))) {}

        Uniform(const glm::vec4& val)
            : type(UniformType::VEC4), ptr(std::static_pointer_cast<void>(std::make_shared<glm::vec4>(val))) {}

        Uniform(const glm::mat3& val)
            : type(UniformType::MAT3), ptr(std::static_pointer_cast<void>(std::make_shared<glm::mat3>(val))) {}

        Uniform(const glm::mat4& val)
            : type(UniformType::MAT4), ptr(std::static_pointer_cast<void>(std::make_shared<glm::mat4>(val))) {}
    };

    // TODO: criar estruturas para usar UBO (uniform block object) https://www.youtube.com/watch?v=s4UHxFjQji4
    // class UniformBlockMem {
    //     std::string name;
    //     std::vector<Uniform> uniforms;
    // };

    using MapUniform = std::unordered_map<std::string, Uniform>;

} // namespace ce
