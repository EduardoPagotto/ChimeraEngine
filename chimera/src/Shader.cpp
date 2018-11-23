#include "Shader.h"
#include "ExceptionChimera.h"

namespace Chimera {

Shader::Shader(const std::string &_programName,const GLuint &_idProgram) : currentProgram(_programName), idProgram(_idProgram) {
    log = spdlog::get("chimera");
    log->debug("Contructor Shader {0} -> {1}", currentProgram, idProgram);
}

Shader::Shader(const Shader &_shader) {

    log = spdlog::get("chimera");
    log->debug("Contructor Copy Shader {0} -> {1}", currentProgram, idProgram);

    idProgram = _shader.idProgram;
    currentProgram = _shader.currentProgram;
}

Shader::~Shader() {
    log->debug("Remove Shader: {0} -> {1} ", currentProgram, idProgram);
    glDeleteProgram(idProgram);
}

GLint Shader::getUniformLocation(const char* _name) const noexcept
{
	// nasty C lib uses -1 return value for error
	GLint loc = glGetUniformLocation(idProgram, _name);
	if (loc == -1)
        log->error("Shader Uniform \"{0}\" not found in Program \"{1}\"", _name, currentProgram);

	return loc;
}

void Shader::setGlUniform1i(const char * _nameVar, const int &_val) const noexcept {
	glUniform1i(getUniformLocation(_nameVar), _val);
}

void Shader::setGlUniform3f(const char *_nameVar, const float &_x, const float &_y, const float &_z) const noexcept {
	glUniform3f(getUniformLocation(_nameVar), _x, _y, _z);
}

void Shader::setGlUniform3fv ( const char* _nameVar, const unsigned& _num, const float* _pointer ) const noexcept {
	glUniform3fv(getUniformLocation(_nameVar), _num, _pointer);
}

void Shader::setGlUniform4fv ( const char* _nameVar, const unsigned &_num, const float *_pointer ) const noexcept {
    glUniform4fv(getUniformLocation(_nameVar), _num, _pointer);
}

void Shader::setGlUniform1fv ( const char* _nameVar, const unsigned& _num, const float* _pointer ) const noexcept {
    glUniform1fv(getUniformLocation(_nameVar), _num, _pointer);
}

void Shader::setGlUniformMatrix4fv ( const char* _nameVar, const unsigned& _num, const bool& _normal,const float* _pointer ) const noexcept {
    glUniformMatrix4fv(getUniformLocation(_nameVar), _num, _normal, _pointer);
}

void Shader::setGlUniformMatrix3fv ( const char* _nameVar, const unsigned& _num, const bool& _normal, const float* _pointer ) const noexcept {
    glUniformMatrix3fv(getUniformLocation(_nameVar), _num, _normal, _pointer);
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
