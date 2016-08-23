#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>

#include "OpenGLDefs.h"

namespace Chimera
{

class Shader {
public:
   Shader();
   Shader(const Shader &_shader);
   virtual ~Shader();

   bool load(const std::string &programName, const std::string &vertex_file_path, const std::string &fragment_file_path);
   
    inline int getErroCode() const {
        return erroCode;
    }

    inline void link()  { 
        glUseProgram(this->idProgram); 
    }
    
    static inline void unlink() {
        glUseProgram(0); 
    }
    
    inline GLuint getIdProgram() {
        return idProgram;
    }
    
	 bool setGlUniform1i(const char *_nameVar, const int &_val);
	 bool setGlUniform3fv( const char *_nameVar, const unsigned &_num, const float *_pointer );
     bool setGlUniform4fv( const char *_nameVar, const unsigned &_num, const float *_pointer ); 
     bool setGlUniform1fv( const char *_nameVar, const unsigned &_num, const float *_pointer );
     bool setGlUniformMatrix4fv( const char* _nameVar, const unsigned& _num,const bool& _normal,const float* _pointer );
     bool setGlUniformMatrix3fv( const char* _nameVar, const unsigned& _num,const bool& _normal,const float* _pointer );
     
private: 
    std::string getShaderCode ( const char * file_path );
    GLuint compileShader ( const std::string &shaderCode, bool _shadeKind );
    GLuint linkShader (const GLuint &VertexShaderID, const GLuint &FragmentShaderID );
   
    int erroCode;
    GLuint idProgram;

	std::map<std::string, GLuint> mapaId;

	std::string currentProgram;
	GLuint currentIdProgram;
};
}

#endif
