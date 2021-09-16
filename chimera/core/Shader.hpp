#ifndef __CHIMERA_SHADER__HPP
#define __CHIMERA_SHADER__HPP

#include "chimera/OpenGLDefs.hpp"
#include <string>

namespace Chimera {
class Shader {
  public: // https://www.youtube.com/watch?v=qTGMXcFLk2E
    Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath);

    virtual ~Shader();

    inline void enable() const { glUseProgram(this->shaderId); }
    inline void disable() const { glUseProgram(0); }
    inline std::string getName() const { return name; }

    GLint getUniformLocation(const char* _varName) const noexcept;

    inline void setUniform1i(const char* _nameVar, const int& _val) const { glUniform1i(getUniformLocation(_nameVar), _val); }

    inline void setUniform1f(const char* _nameVar, const float& _val) const { glUniform1f(getUniformLocation(_nameVar), _val); }

    inline void setUniform2f(const char* _nameVar, const float& _x, const float& _y) const {
        glUniform2f(getUniformLocation(_nameVar), _x, _y);
    }

    inline void setUniform3f(const char* _nameVar, const float& _x, const float& _y, const float& _z) const {
        glUniform3f(getUniformLocation(_nameVar), _x, _y, _z);
    }

    inline void setUniform1fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const {
        glUniform1fv(getUniformLocation(_nameVar), _num, _pointer);
    }

    inline void setUniform2fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const {
        glUniform2fv(getUniformLocation(_nameVar), _num, _pointer);
    }

    inline void setUniform3fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const {
        glUniform3fv(getUniformLocation(_nameVar), _num, _pointer);
    }

    inline void setUniform4fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const {
        glUniform4fv(getUniformLocation(_nameVar), _num, _pointer);
    }

    inline void setUniformMatrix4fv(const char* _nameVar, const unsigned& _num, const bool& _normal, const float* _pointer) const {
        glUniformMatrix4fv(getUniformLocation(_nameVar), _num, _normal, _pointer);
    }

    inline void setUniformMatrix3fv(const char* _nameVar, const unsigned& _num, const bool& _normal, const float* _pointer) const {
        glUniformMatrix3fv(getUniformLocation(_nameVar), _num, _normal, _pointer);
    }

  private:
    GLuint shaderId;
    std::string name;
};

} // namespace Chimera
#endif
