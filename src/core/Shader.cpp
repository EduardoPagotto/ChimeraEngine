#include <string>

#include "chimera/core/Exception.hpp"
#include "chimera/core/Shader.hpp"

namespace Chimera {

Shader::Shader(const std::string& _programName, const GLuint& _idProgram)
    : currentProgram(_programName), idProgram(_idProgram) {}

Shader::Shader(const Shader& _shader) {
    idProgram = _shader.idProgram;
    currentProgram = _shader.currentProgram;
}

Shader::~Shader() { glDeleteProgram(idProgram); }

GLint Shader::getUniformLocation(const char* _name) const noexcept {
    // nasty C lib uses -1 return value for error
    GLint loc = glGetUniformLocation(idProgram, _name);
    if (loc == -1)
        printf("Shader Uniform \"%s\" not found in Program \"%s\"", _name, currentProgram.c_str());

    return loc;
}

void Shader::setGlUniform1i(const char* _nameVar, const int& _val) const noexcept {
    glUniform1i(getUniformLocation(_nameVar), _val);
}

void Shader::setGlUniform3f(const char* _nameVar, const float& _x, const float& _y, const float& _z) const noexcept {
    glUniform3f(getUniformLocation(_nameVar), _x, _y, _z);
}

void Shader::setGlUniform3fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept {
    glUniform3fv(getUniformLocation(_nameVar), _num, _pointer);
}

void Shader::setGlUniform4fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept {
    glUniform4fv(getUniformLocation(_nameVar), _num, _pointer);
}

void Shader::setGlUniform1fv(const char* _nameVar, const unsigned& _num, const float* _pointer) const noexcept {
    glUniform1fv(getUniformLocation(_nameVar), _num, _pointer);
}

void Shader::setGlUniformMatrix4fv(const char* _nameVar, const unsigned& _num, const bool& _normal,
                                   const float* _pointer) const noexcept {
    glUniformMatrix4fv(getUniformLocation(_nameVar), _num, _normal, _pointer);
}

void Shader::setGlUniformMatrix3fv(const char* _nameVar, const unsigned& _num, const bool& _normal,
                                   const float* _pointer) const noexcept {
    glUniformMatrix3fv(getUniformLocation(_nameVar), _num, _normal, _pointer);
}
} // namespace Chimera
