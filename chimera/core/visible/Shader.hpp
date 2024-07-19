#pragma once
#include "UValue.hpp"

namespace Chimera {

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

struct ShaderComponent {
    ShaderComponent(const std::string& name, std::shared_ptr<Shader> shader) : name(name), shader(shader) {}
    virtual ~ShaderComponent() = default;
    std::string name;
    std::shared_ptr<Shader> shader;
};

//---

class ShaderManager {
  private:
    inline static std::unordered_map<std::string, std::shared_ptr<Shader>> map_shaders;

  public:
    static std::shared_ptr<Shader> load(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles) noexcept;
    // static bool remove(const std::string& name) noexcept;
    static void clear() noexcept;
};
} // namespace Chimera
