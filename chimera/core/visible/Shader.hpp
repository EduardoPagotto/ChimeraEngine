#pragma once
#include "UValue.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Chimera {

class Shader {
    friend class ShaderManager;

  public:
    Shader() = default;
    Shader(const Shader& other) : progID(other.progID) {
        uniformLocationCache.insert(other.uniformLocationCache.begin(), other.uniformLocationCache.end());
    }

    Shader& operator=(const Shader& other) {
        this->progID = other.progID;
        uniformLocationCache.insert(other.uniformLocationCache.begin(), other.uniformLocationCache.end());
        return *this;
    }

    virtual ~Shader() = default;
    inline const GLuint getID() const { return this->progID; }
    inline void invalidade();
    const GLint getUniform(const std::string& name) const noexcept;
    void setUniformU(const char* name, const UValue& uv);
    inline const bool isInvalid() const { return progID == 0; }
    bool operator==(const Shader& other) const { return progID == other.progID; }
    bool operator!=(const Shader& other) const { return !(*this == other); }

  private:
    GLuint progID = 0;
    mutable std::unordered_map<std::string, GLint> uniformLocationCache;
};
//---

class ShaderManager {
  public:
    static void load(const std::string& name, const std::unordered_map<GLenum, std::string>& mFiles, Shader& shader);
    static const Shader& get(const std::string& name);
    static bool remove(const std::string& name);
    static void clear();

  private:
    inline static std::unordered_map<std::string, Shader> mShaders;
};
} // namespace Chimera
