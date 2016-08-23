#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Shader.h"
#include "ExceptionChimera.h"

namespace Chimera {

Shader::Shader () : erroCode(0) , idProgram(0) {}

Shader::Shader ( const Shader& _shader ) : erroCode(_shader.erroCode), idProgram(_shader.idProgram) {}

Shader::~Shader() {}
        
bool Shader::load ( const char* vertex_file_path, const char* fragment_file_path ) {
    
    //Ler os arquivos
    std::string VertexShaderCode = getShaderCode ( vertex_file_path );
    std::string FragmentShaderCode = getShaderCode ( fragment_file_path );

    // Compila Vertex Shader
    std::cout << "Compiling Vertex Shader :" << vertex_file_path << std::endl;
    GLuint VertexShaderID = compileShader(VertexShaderCode, true);
    
    // Compila Fragment Shader
    std::cout << "Compiling Fragment Shader : " << std::string ( fragment_file_path ) << std::endl;
    GLuint FragmentShaderID = compileShader(FragmentShaderCode, false);

    // Link o programa
    std::cout << "Linking program " << std::endl;
    idProgram = linkShader ( VertexShaderID, FragmentShaderID );

    glDeleteShader ( VertexShaderID );
    glDeleteShader ( FragmentShaderID );
    
    return erroCode != 0 ? false :true;
}

std::string Shader::getShaderCode ( const char * file_path ) {

    // Read the Vertex Shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream ( file_path, std::ios::in );
    if ( shaderStream.is_open() ) {
        std::string Line = "";
        while ( getline ( shaderStream, Line ) ) {
            shaderCode += "\n" + Line;
        }

        shaderStream.close();

    } else {
        erroCode = -1;
        throw  ExceptionChimera ( ExceptionCode::OPEN, "Impossivel abrir arquivo: " + std::string ( file_path ) );
    }

    return shaderCode;
}

GLuint Shader::compileShader ( const std::string &shaderCode, bool _shadeKind ) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderID = -1;
    if (_shadeKind == false)
        shaderID = glCreateShader ( GL_FRAGMENT_SHADER );
    else
        shaderID = glCreateShader ( GL_VERTEX_SHADER );
    
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource ( shaderID, 1, &sourcePointer, NULL );
    glCompileShader ( shaderID );

    // Check Fragment Shader
    glGetShaderiv ( shaderID, GL_COMPILE_STATUS, &Result );
    
    if (Result == GL_FALSE) {
        glGetShaderiv ( shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
        if ( InfoLogLength > 0 ) {
            std::vector<char> shaderErrorMessage ( InfoLogLength + 1 );
            glGetShaderInfoLog ( shaderID, InfoLogLength, NULL, &shaderErrorMessage[0] );

            std::cout << "Check Fragment Shader " << std::string ( &shaderErrorMessage[0] ) << std::endl;
            erroCode = -2; //falha compilar
        }
    }

    return shaderID;
}

GLuint Shader::linkShader (const GLuint &VertexShaderID, const GLuint &FragmentShaderID ) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint ProgramID = glCreateProgram();
    glAttachShader ( ProgramID, VertexShaderID );
    glAttachShader ( ProgramID, FragmentShaderID );
    glLinkProgram ( ProgramID );

    // Check the program
    glGetProgramiv ( ProgramID, GL_LINK_STATUS, &Result );
    
    if (Result == GL_FALSE) {
    
        glGetProgramiv ( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
        if ( InfoLogLength > 0 ) {
            std::vector<char> ProgramErrorMessage ( InfoLogLength + 1 );
            glGetProgramInfoLog ( ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0] );

            std::cout << "Check program " << std::string ( &ProgramErrorMessage[0] ) << std::endl;
            erroCode = -3; //falha link
        }
    }
    return ProgramID;
}

bool Shader::setGlUniform1i(const char * _nameVar, const int &_val)
{
	GLint loc = glGetUniformLocation(idProgram, _nameVar);
	if (loc >= 0) {
		glUniform1i(loc, (GLint)_val);
		return true;
	}

	return false;
}

bool Shader::setGlUniform3fv ( const char* _nameVar, const unsigned& _num, const float* _pointer ) {
    
     GLint loc  = glGetUniformLocation( idProgram, _nameVar );
     if ( loc >= 0 ) {
        glUniform3fv(loc, _num, _pointer);
        return true;
     }
    
    return false;
}


bool Shader::setGlUniform4fv ( const char* _nameVar, const unsigned &_num, const float *_pointer ) {

     GLint loc  = glGetUniformLocation( idProgram, _nameVar );
     if ( loc >= 0 ) {
        glUniform4fv(loc, _num, _pointer);
        return true;
     }
    
    return false;
}

bool Shader::setGlUniform1fv ( const char* _nameVar, const unsigned& _num, float* _pointer ) {
    
     GLint loc  = glGetUniformLocation( idProgram, _nameVar );
     if ( loc >= 0 ) {
        glUniform1fv(loc, _num, _pointer);
        return true;
     }
    
    return false;
}

bool Shader::setGlUniformMatrix4fv ( const char* _nameVar, const unsigned& _num, const bool& _normal,const float* _pointer ) {

    GLint ploc = glGetUniformLocation( idProgram, _nameVar );
    if (ploc >= 0) {
        glUniformMatrix4fv(ploc, _num, _normal, _pointer);
        return true;
    }
    
    return false;
}

bool Shader::setGlUniformMatrix3fv ( const char* _nameVar, const unsigned& _num, const bool& _normal, const float* _pointer ) {
    
    GLint ploc = glGetUniformLocation( idProgram, _nameVar );
    if (ploc >= 0) {
        glUniformMatrix3fv(ploc, _num, _normal, _pointer);
        return true;
    }
    
    return false;
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
