#include <vector>
#include <iostream>
#include <fstream>

#include "Shader.h"
#include "ExceptionChimera.h"

namespace Chimera {

Shader::Shader() noexcept {
    idProgram = 0;
    currentProgram = "default";
    log = spdlog::get("chimera");
    log->debug("Contructor Shader");
}

void Shader::reset() noexcept {
    log->debug("Destructor Shader");  

    //std::cerr<<"Closing down shader manager\n";
    // TODO: Implementar finalização do shader

    //   for(auto programs : m_shaderPrograms)
    //     delete programs.second;
    //   for(auto shader : m_shaders)
    //     delete shader.second;
}

bool Shader::load (const std::string &programName, const std::string &vertex_file_path, const std::string &fragment_file_path) {

	bool retorno = false;

    //Ler os arquivos
    std::string VertexShaderCode = getShaderCode ( vertex_file_path.c_str() );
    std::string FragmentShaderCode = getShaderCode ( fragment_file_path.c_str() );

    log->debug("Shader name:{}", programName);

    // Compila Vertex Shader
    log->debug("Compiling Vertex:{}", vertex_file_path);
    GLuint VertexShaderID = compileShader(VertexShaderCode, true);

    // Compila Fragment Shader
    log->debug("Compiling Fragment:{}", fragment_file_path);
    GLuint FragmentShaderID = compileShader(FragmentShaderCode, false);

    // Link o programa
    idProgram = linkShader ( VertexShaderID, FragmentShaderID );
    log->debug("Linked: {0:03d}", idProgram);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	mapaId[programName] = idProgram;
	currentProgram = programName;

	if (idProgram != -1) {
        log->info("Shader OK:{0} Id:{1:03d}", programName, idProgram);
		retorno = true;
	}
	else {
        log->error("Shader Erro: {}", programName);
	}

	return retorno;
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
            log->error("Shader Check Fragment Shader: {}", std::string ( &shaderErrorMessage[0] ));
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
            log->error("Shader Check program: {}", std::string ( &ProgramErrorMessage[0] ));
        }
    }
    return ProgramID;
}

bool Shader::selectCurrent(const std::string & _cur)
{
	auto var = mapaId.find(_cur);
	if (var != mapaId.end()) {
		currentProgram = var->first;
		idProgram = var->second;
		return true;
	}

    log->error("Shader Invalido: {0} Id:{1:03d}", _cur, idProgram);
	return false;
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
