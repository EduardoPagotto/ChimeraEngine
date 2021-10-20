#ifndef __CHIMERA_SHADER__HPP
#define __CHIMERA_SHADER__HPP

#include "chimera/core/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

namespace Chimera {
class Shader {
    friend class ShaderManager;

  public:
    Shader() = default;
    Shader(const Shader& other) : shaderId(other.shaderId), name(other.name) {}
    virtual ~Shader() = default;

    inline void enable() const { glUseProgram(this->shaderId); }
    static void disable() { glUseProgram(0); }
    inline const std::string getName() const { return name; }
    GLint getUniform(const char* _varName) const noexcept;
    void setUniform(const char* name, float val) { glUniform1f(getUniform(name), val); }
    void setUniform(const char* name, int val) { glUniform1i(getUniform(name), val); }
    void setUniform(const char* name, const glm::vec2& vec) { glUniform2f(getUniform(name), vec.x, vec.y); }
    void setUniform(const char* name, const glm::ivec2& vec) { glUniform2i(getUniform(name), vec.x, vec.y); }
    void setUniform(const char* name, const glm::vec3& vec) { glUniform3f(getUniform(name), vec.x, vec.y, vec.z); }
    void setUniform(const char* name, const glm::ivec3& vec) { glUniform3i(getUniform(name), vec.x, vec.y, vec.z); }
    void setUniform(const char* name, const glm::vec4& vec) { glUniform4f(getUniform(name), vec.x, vec.y, vec.z, vec.w); }
    void setUniform(const char* name, const glm::ivec4& vec) { glUniform4i(getUniform(name), vec.x, vec.y, vec.z, vec.w); }
    void setUniform(const char* name, const glm::mat3& mat) { glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat)); }
    void setUniform(const char* name, const glm::mat4& mat) { glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat)); }

    void setUniformArray(const char* name, int size, float* val) { glUniform1fv(getUniform(name), size, val); }
    void setUniformArray(const char* name, int size, int* val) { glUniform1iv(getUniform(name), size, val); }
    void setUniformArray(const char* name, int size, glm::vec2* val) { glUniform2fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec2* val) { glUniform2iv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::vec3* val) { glUniform3fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec3* val) { glUniform3iv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::vec4* val) { glUniform4fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec4* val) { glUniform4iv(getUniform(name), size, glm::value_ptr(*val)); }
    inline const bool isInvalid() const { return shaderId == 0; }
    bool operator==(const Shader& other) const { return shaderId == other.shaderId; } //&& name == other.name; }
    bool operator!=(const Shader& other) const { return !(*this == other); }

  private:
    GLuint shaderId = 0;
    std::string name = "invalid";
};
//---
class ShaderValue {
  public:
    ShaderValue(const std::string& name, const float& value) : name(name), value(glm::vec4(value)), isFloat(true) {}
    ShaderValue(const std::string& name, const glm::vec4& value) : name(name), value(value), isFloat(false) {}
    ShaderValue(const ShaderValue& cpy) : name(cpy.name), value(cpy.value), isFloat(cpy.isFloat) {}

    void setUniform(Shader* _shader) {
        if (isFloat)
            _shader->setUniform(name.c_str(), value[0]);
        else
            _shader->setUniform(name.c_str(), value);
    }

  private:
    bool isFloat;
    std::string name;
    glm::vec4 value;
};
//---
class ShaderManager {
  public:
    static void load(const std::string& filepath, Shader& shader);
    static bool get(const std::string& name, Shader& shader);
    static bool remove(Shader& shader);
    static void clear();

  private:
    static std::vector<Shader> shaders;
};
} // namespace Chimera
#endif
