#include <SDL2/SDL.h>
//#include <fstream>
#include <string>
#include <vector>

#include "chimera/core/Exception.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/io/utils.hpp"

namespace Chimera {

// --- shade internal func
GLuint compileShader(const std::string& shaderCode, uint16_t kindShade) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderID = -1;
    shaderID = glCreateShader(kindShade);

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
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader Check Fragment Shader: %s", std::string(&shaderErrorMessage[0]).c_str());
            throw Exception(std::string("compileShader Fail: ") + shaderCode);
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
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader Check program: %s", std::string(&ProgramErrorMessage[0]).c_str());
        }

        throw Exception(std::string("linkShader Fail"));
    }
    return ProgramID;
}

GLuint shadeLoadProg(const char* progName, const char* fileVertex, const char* fileFrag) {

    GLuint VertexShaderID = compileShader(IO::utilReadFile(fileVertex), GL_VERTEX_SHADER);
    GLuint FragmentShaderID = compileShader(IO::utilReadFile(fileFrag), GL_FRAGMENT_SHADER);

    // Link o programa
    GLuint shaderId = linkShader(VertexShaderID, FragmentShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    if (shaderId > 0)
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader %s id: %d", progName, shaderId);
    else {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader %s", progName);
        throw Exception(std::string("shadeLoadProg Fail"));
    }

    return shaderId;
}

Shader::Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath) : name(name) {
    this->shaderId = shadeLoadProg(name.c_str(), vertPath.c_str(), fragPath.c_str());
}

Shader::~Shader() { glDeleteProgram(shaderId); }

GLint Shader::getUniformLocation(const char* _varName) const noexcept {
    // nasty C lib uses -1 return value for error
    GLint loc = glGetUniformLocation(shaderId, _varName);
    if (loc == -1)
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%s\"", _varName, name.c_str());

    return loc;
}
} // namespace Chimera
