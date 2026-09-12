#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace ce {
    struct Triangle {
        bool splitter{false};
        glm::uvec3 idx{0};
        glm::vec3 normal{0.0};

        explicit Triangle(const Triangle& t) noexcept = default;
        explicit Triangle(const glm::uvec3& i, const glm::vec3& n, const bool& s) noexcept
            : splitter(s), idx(i), normal(n) {}
    };

    using TrisIndex = std::vector<glm::uvec3>;
} // namespace ce
