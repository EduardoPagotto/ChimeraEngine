#pragma once
#include "chimera/core/UValue.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Chimera {

class Shader {
    friend class ShaderManager;

  public:
    Shader() = default;
    Shader(const Shader& other) : progID(other.progID) {}
    virtual ~Shader() = default;
    inline const GLuint getID() const { return this->progID; }
    inline void invalidade();
    const GLint getUniform(const char* _varName) const noexcept;
    void setUniform(const char* name, float val) const { glUniform1f(getUniform(name), val); }
    void setUniform(const char* name, int val) const { glUniform1i(getUniform(name), val); }
    void setUniform(const char* name, const glm::vec2& vec) const { glUniform2f(getUniform(name), vec.x, vec.y); }
    void setUniform(const char* name, const glm::ivec2& vec) const { glUniform2i(getUniform(name), vec.x, vec.y); }
    void setUniform(const char* name, const glm::vec3& vec) const { glUniform3f(getUniform(name), vec.x, vec.y, vec.z); }
    void setUniform(const char* name, const glm::ivec3& vec) const { glUniform3i(getUniform(name), vec.x, vec.y, vec.z); }
    void setUniform(const char* name, const glm::vec4& vec) const { glUniform4f(getUniform(name), vec.x, vec.y, vec.z, vec.w); }
    void setUniform(const char* name, const glm::ivec4& vec) const { glUniform4i(getUniform(name), vec.x, vec.y, vec.z, vec.w); }
    void setUniform(const char* name, const glm::mat3& mat) const {
        glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setUniform(const char* name, const glm::mat4& mat) const {
        glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setUniform(const UValue& uv);
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
};
//---

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
