#pragma once
#include "UValue.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

class Shader {
    friend class ShaderManager;

  private:
    GLuint progID{0};
    mutable std::unordered_map<std::string, GLint> uniformLocationCache;

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
    inline const GLuint getID() const noexcept { return this->progID; }
    inline void invalidade() noexcept {
        glDeleteProgram(progID);
        progID = 0;
        uniformLocationCache.clear();
    }
    void setUniformU(const char* name, const UValue& uv) noexcept;

    inline const GLint getUniform(const std::string& name) const noexcept {

        if (uniformLocationCache.find(name) != uniformLocationCache.end())
            return uniformLocationCache[name];

        GLint loc = glGetUniformLocation(progID, name.c_str());
        uniformLocationCache[name] = loc;

        return loc;
    }

    inline const bool isInvalid() const { return progID == 0; }
};
//---

class ShaderManager {
  private:
    inline static std::unordered_map<std::string, Shader> mShaders;

  public:
    static void load(const std::string& name, const std::unordered_map<GLenum, std::string>& mFiles, Shader& shader) noexcept;
    static bool remove(const std::string& name) noexcept;
    static void clear() noexcept;
};
} // namespace Chimera
