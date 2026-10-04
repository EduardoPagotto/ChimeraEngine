#pragma once
#include "chimera_base/aux/Uniform.hpp"

namespace ce {

    class Shader {
      public:
        explicit Shader(const uint32_t& id) noexcept : id_(id) {}

        Shader() = delete;
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

        virtual ~Shader() noexcept;

        bool operator==(const Shader& other) const noexcept { return id_ == other.id_; }
        bool operator!=(const Shader& other) const noexcept { return !(*this == other); }
        uint32_t get_id() const noexcept { return this->id_; }
        int32_t get_uniform(const std::string& name) const noexcept;
        void set_uniform_u(const char* name, const Uniform& uv) const noexcept;

      private:
        uint32_t id_{0};
        mutable std::unordered_map<std::string, int32_t> uniform_location_cache_;
    };
} // namespace ce
