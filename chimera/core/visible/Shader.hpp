#pragma once
#include "UValue.hpp"

namespace Chimera {

class Shader {
    friend class ShaderManager;

  private:
    uint32_t progID{0};
    mutable std::unordered_map<std::string, int32_t> uniformLocationCache;

  public:
    Shader() noexcept = default;
    Shader(const Shader& other) noexcept : progID(other.progID) {
        uniformLocationCache.insert(other.uniformLocationCache.begin(), other.uniformLocationCache.end());
    }

    Shader& operator=(const Shader& other) noexcept {
        this->progID = other.progID;
        uniformLocationCache.insert(other.uniformLocationCache.begin(), other.uniformLocationCache.end());
        return *this;
    }

    virtual ~Shader() noexcept = default;

    inline const bool operator==(const Shader& other) const noexcept { return progID == other.progID; }
    inline const bool operator!=(const Shader& other) const noexcept { return !(*this == other); }
    inline const uint32_t getID() const noexcept { return this->progID; }

    void invalidade() noexcept;
    void setUniformU(const char* name, const UValue& uv) noexcept;

    const int32_t getUniform(const std::string& name) const noexcept;
    const bool isInvalid() const { return progID == 0; }
};
//---

class ShaderManager {
  private:
    inline static std::unordered_map<std::string, Shader> mShaders;

  public:
    static void load(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles, Shader& shader) noexcept;
    static bool remove(const std::string& name) noexcept;
    static void clear() noexcept;
};
} // namespace Chimera
