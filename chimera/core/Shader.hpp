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
    void setGlUniform1i(const char* _nameVar, const int& _val) const noexcept;
    void setGlUniform1f(const char* _nameVar, const float& _val) const noexcept;
    void setGlUniform3f(const char* _nameVar, const float& _x, const float& _y, const float& _z) const noexcept;
    void setGlUniform3fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept;
    void setGlUniform4fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept;
    void setGlUniform1fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept;
    void setGlUniformMatrix4fv(const char* _nameVar, const unsigned& _num, const bool& _normal, const float* _pointer) const noexcept;
    void setGlUniformMatrix3fv(const char* _nameVar, const unsigned& _num, const bool& _normal, const float* _pointer) const noexcept;

  private:
    GLuint shaderId;
    std::string name;
};

} // namespace Chimera
#endif
