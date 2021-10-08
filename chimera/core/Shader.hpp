#ifndef __CHIMERA_SHADER__HPP
#define __CHIMERA_SHADER__HPP

#include "chimera/core/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

namespace Chimera {
class Shader {
  public:
    Shader(const std::string& filepath);
    Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
    virtual ~Shader();

    inline void enable() const { glUseProgram(this->shaderId); }
    inline void disable() const { glUseProgram(0); }
    inline std::string getName() const { return name; }

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

  private:
    GLuint shaderId;
    std::string name;
};
} // namespace Chimera
#endif
