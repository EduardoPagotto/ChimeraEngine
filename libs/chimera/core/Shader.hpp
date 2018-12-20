#ifndef __CHIMERA_SHADER__HPP
#define __CHIMERA_SHADER__HPP

#include "chimera/core/OpenGLDefs.hpp"
#include <string.h>

namespace Chimera {
class Shader {
  public:
    Shader(const std::string& _programName, const GLuint& _idProgram);

    Shader(const Shader& _shader);

    virtual ~Shader();

    inline void link() { glUseProgram(this->idProgram); }

    std::string getCurrentProgram() const { return currentProgram; }

    GLuint getIdProgram() const { return idProgram; }

    static inline void unlink() { glUseProgram(0); }

    GLint getUniformLocation(const char* _name) const noexcept;
    void setGlUniform1i(const char* _nameVar, const int& _val) const noexcept;
    void setGlUniform3f(const char* _nameVar, const float& _x, const float& _y, const float& _z) const noexcept;
    void setGlUniform3fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept;
    void setGlUniform4fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept;
    void setGlUniform1fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept;
    void setGlUniformMatrix4fv(const char* _nameVar, const unsigned& _num, const bool& _normal,
                               const float* _pointer) const noexcept;
    void setGlUniformMatrix3fv(const char* _nameVar, const unsigned& _num, const bool& _normal,
                               const float* _pointer) const noexcept;

  private:
    GLuint idProgram;
    std::string currentProgram;
};
} // namespace Chimera
#endif
