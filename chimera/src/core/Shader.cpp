#include <SDL2/SDL.h>
#include <fstream>
#include <string>
#include <vector>

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
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%s\"", _name,
                     currentProgram.c_str());
    return loc;
}

void Shader::setGlUniform1i(const char* _nameVar, const int& _val) const noexcept {
    glUniform1i(getUniformLocation(_nameVar), _val);
}

void Shader::setGlUniform1f(const char* _nameVar, const float& _val) const noexcept {
    glUniform1f(getUniformLocation(_nameVar), _val);
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

// --- shade
std::string getShaderCode(const char* file_path) {

    // Read the Vertex Shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file_path, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }
        shaderStream.close();
    } else {
        throw Chimera::Exception("Impossivel abrir arquivo: " + std::string(file_path));
    }

    return shaderCode;
}

GLuint compileShader(const std::string& shaderCode, bool _shadeKind) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderID = -1;
    if (_shadeKind == false)
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    else
        shaderID = glCreateShader(GL_VERTEX_SHADER);

    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // Check Fragment Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);

    if (Result == GL_FALSE) {
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> shaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader Check Fragment Shader: %s",
                         std::string(&shaderErrorMessage[0]).c_str());
        }
    }

    return shaderID;
}

GLuint linkShader(const GLuint& VertexShaderID, const GLuint& FragmentShaderID) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);

    if (Result == GL_FALSE) {

        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader Check program: %s",
                         std::string(&ProgramErrorMessage[0]).c_str());
        }
    }
    return ProgramID;
}

GLuint shadeLoadProg(const char* programName, const char* vertex_file_path, const char* fragment_file_path) {

    GLuint VertexShaderID = compileShader(getShaderCode(vertex_file_path), true);
    GLuint FragmentShaderID = compileShader(getShaderCode(fragment_file_path), false);

    // Link o programa
    GLuint idProgram = linkShader(VertexShaderID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    if (idProgram != -1)
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader %s id: %d", programName, idProgram);
    else
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader %s", programName);

    return idProgram;
}

// Shader* loadShader(const std::string& programName, const std::string& vertex_file_path,
//                    const std::string& fragment_file_path) {
//     GLuint id = load(programName, vertex_file_path, fragment_file_path);
//     return new Shader(programName, id);
// }

} // namespace Chimera
