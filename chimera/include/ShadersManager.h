#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>

#include "Singleton.h"
#include "OpenGLDefs.h"

#include <spdlog/spdlog.h>

namespace Chimera
{

class ShadersManager {
	friend class Singleton<ShadersManager >;
public:

	bool load(const std::string &programName, const std::string &vertex_file_path, const std::string &fragment_file_path);

	inline void link()  {
		glUseProgram(this->idProgram);
	}

	std::string getCurrent() const {
		return currentProgram;
	}

	static inline void unlink() {
		glUseProgram(0);
	}

	void reset() noexcept;

	bool selectCurrent(const std::string &_cur);
	GLint getUniformLocation(const char* _name) const noexcept;
	void setGlUniform1i(const char * _nameVar, const int &_val) const noexcept;
	void setGlUniform3f(const char *_nameVar, const float &_x, const float &_y, const float &_z) const noexcept;
	void setGlUniform3fv( const char *_nameVar, const unsigned &_num, const float *_pointer ) const noexcept;
	void setGlUniform4fv( const char *_nameVar, const unsigned &_num, const float *_pointer ) const noexcept;
	void setGlUniform1fv( const char *_nameVar, const unsigned &_num, const float *_pointer ) const noexcept;
	void setGlUniformMatrix4fv( const char* _nameVar, const unsigned& _num,const bool& _normal,const float* _pointer ) const noexcept;
	void setGlUniformMatrix3fv( const char* _nameVar, const unsigned& _num,const bool& _normal,const float* _pointer )const noexcept;

protected:
	ShadersManager() noexcept;

	virtual ~ShadersManager() {
		reset();
	}

	ShadersManager(const ShadersManager&)=delete;

private:
    std::string getShaderCode ( const char * file_path );
    GLuint compileShader ( const std::string &shaderCode, bool _shadeKind );
    GLuint linkShader (const GLuint &VertexShaderID, const GLuint &FragmentShaderID );

    GLuint idProgram;
	std::map<std::string, GLuint> mapaId;
	std::string currentProgram;
	std::shared_ptr<spdlog::logger> log;
};
}

#endif
