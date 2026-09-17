#pragma once
#include "chimera_base/aux/Uniform.hpp"

namespace ce {

    class Shader {
      public:
        explicit Shader(const uint32_t& id) noexcept : id(id) {}

        Shader() = delete;
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

        virtual ~Shader() noexcept;

        bool operator==(const Shader& other) const noexcept { return id == other.id; }
        bool operator!=(const Shader& other) const noexcept { return !(*this == other); }
        uint32_t getID() const noexcept { return this->id; }
        int32_t getUniform(const std::string& name) const noexcept;
        void setUniformU(const char* name, const Uniform& uv) const noexcept;

      private:
        uint32_t id{0};
        mutable std::unordered_map<std::string, int32_t> uniformLocationCache;
    };
} // namespace ce
