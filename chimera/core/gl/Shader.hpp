#pragma once
#include "chimera/base/UValue.hpp"

namespace ce {

    class Shader {
      private:
        uint32_t progID{0};
        mutable std::unordered_map<std::string, int32_t> uniformLocationCache;

      public:
        explicit Shader(const uint32_t& id) noexcept : progID(id) {}

        Shader() = delete;
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

        virtual ~Shader() noexcept;

        const bool operator==(const Shader& other) const noexcept { return progID == other.progID; }
        const bool operator!=(const Shader& other) const noexcept { return !(*this == other); }
        const uint32_t getID() const noexcept { return this->progID; }
        const int32_t getUniform(const std::string& name) const noexcept;
        void setUniformU(const char* name, const UValue& uv) noexcept;
    };
} // namespace ce
