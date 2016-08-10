#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include <OpenGLDefs.h>

namespace Chimera
{

class Shader {
public:
   Shader();
   Shader(const Shader &_shader);
   virtual ~Shader();
   
    inline bool hasError() const {
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
    
     bool load(const char * vertex_file_path, const char * fragment_file_path);
    
private: 
    std::string getShaderCode ( const char * file_path );
    GLuint compileShader ( const std::string &shaderCode, bool _shadeKind );
    GLuint linkShader (const GLuint &VertexShaderID, const GLuint &FragmentShaderID );
    
    int erroCode;;
    GLuint idProgram;

};
}

#endif
