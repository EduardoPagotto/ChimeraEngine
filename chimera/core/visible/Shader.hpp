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
    virtual ~Shader() = default;
    inline const GLuint getID() const { return this->progID; }
    inline void invalidade();
    const GLint getUniform(const std::string& name) const noexcept;
    void setUniformU(const char* name, const UValue& uv);
    void setUniformArray(const char* name, int size, float* val) const { glUniform1fv(getUniform(name), size, val); }
    void setUniformArray(const char* name, int size, int* val) const { glUniform1iv(getUniform(name), size, val); }
    void setUniformArray(const char* name, int size, glm::vec2* val) const { glUniform2fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec2* val) const { glUniform2iv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::vec3* val) const { glUniform3fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec3* val) const { glUniform3iv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::vec4* val) const { glUniform4fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec4* val) const { glUniform4iv(getUniform(name), size, glm::value_ptr(*val)); }
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
    static const Shader get(const std::string& name);
    static bool remove(const std::string& name);
    static void clear();

  private:
    static std::unordered_map<std::string, Shader> mShaders;
};
} // namespace Chimera
