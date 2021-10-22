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
class UniformVal {
  public:
    UniformVal(const std::string& name, const int& value) : name(name), typeVal(0), val_int(value) {}
    UniformVal(const std::string& name, const float& value) : name(name), typeVal(1), val_float(value) {}
    UniformVal(const std::string& name, const glm::vec2& value) : name(name), typeVal(2), val_vec2(value) {}
    UniformVal(const std::string& name, const glm::ivec2& value) : name(name), typeVal(3), val_ivec2(value) {}
    UniformVal(const std::string& name, const glm::vec3& value) : name(name), typeVal(4), val_vec3(value) {}
    UniformVal(const std::string& name, const glm::ivec3& value) : name(name), typeVal(5), val_ivec3(value) {}
    UniformVal(const std::string& name, const glm::vec4& value) : name(name), typeVal(6), val_vec4(value) {}
    UniformVal(const std::string& name, const glm::ivec4& value) : name(name), typeVal(7), val_ivec4(value) {}
    UniformVal(const std::string& name, const glm::mat3& value) : name(name), typeVal(8), val_mat3(value) {}
    UniformVal(const std::string& name, const glm::mat4& value) : name(name), typeVal(9), val_mat4(value) {}

    UniformVal(const UniformVal& cpy) : name(cpy.name), typeVal(cpy.typeVal), val_mat4(cpy.val_mat4) {}

    void setUniform(Shader* _shader) const;

  private:
    uint8_t typeVal;
    std::string name;

    union {
        int val_int;
        float val_float;
        glm::vec2 val_vec2;
        glm::ivec2 val_ivec2;
        glm::vec3 val_vec3;
        glm::ivec3 val_ivec3;
        glm::vec4 val_vec4;
        glm::ivec4 val_ivec4;
        glm::mat3 val_mat3;
        glm::mat4 val_mat4;
    };
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
