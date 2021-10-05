#ifndef __CHIMERA_SHADER__HPP
#define __CHIMERA_SHADER__HPP

#include "chimera/core/OpenGLDefs.hpp"
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

    inline void setUniform1i(const char* name, const int& _val) const { glUniform1i(getUniform(name), _val); }
    inline void setUniform1f(const char* name, const float& _val) const { glUniform1f(getUniform(name), _val); }
    inline void setUniform2f(const char* name, const float& _x, const float& _y) const { glUniform2f(getUniform(name), _x, _y); }
    inline void setUniform3f(const char* name, const float& _x, const float& _y, const float& _z) const {
        glUniform3f(getUniform(name), _x, _y, _z);
    }
    inline void setUniform1iv(const char* name, const int size, const int* _pointer) const {
        glUniform1iv(getUniform(name), size, _pointer);
    }
    inline void setUniform1fv(const char* name, const unsigned& _num, const float* _pointer) const {
        glUniform1fv(getUniform(name), _num, _pointer);
    }
    inline void setUniform2fv(const char* name, const unsigned& _num, const float* _pointer) const {
        glUniform2fv(getUniform(name), _num, _pointer);
    }
    inline void setUniform3fv(const char* name, const unsigned& _num, const float* _pointer) const {
        glUniform3fv(getUniform(name), _num, _pointer);
    }
    inline void setUniform4fv(const char* name, const unsigned& _num, const float* _pointer) const {
        glUniform4fv(getUniform(name), _num, _pointer);
    }
    inline void setUniformMatrix4fv(const char* name, const unsigned& _num, const bool& _normal, const float* _pointer) const {
        glUniformMatrix4fv(getUniform(name), _num, _normal, _pointer);
    }
    inline void setUniformMatrix3fv(const char* name, const unsigned& _num, const bool& _normal, const float* _pointer) const {
        glUniformMatrix3fv(getUniform(name), _num, _normal, _pointer);
    }

  private:
    GLuint shaderId;
    std::string name;
};

} // namespace Chimera
#endif
