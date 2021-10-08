#include "chimera/core/Shader.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

namespace Chimera {

// --- shade internal func
static GLenum shaderTypeFromString(const std::string type) {
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Unknow shader type \"%s\"", type.c_str());

    return 0;
}

static GLuint compileShader(const std::string& shaderCode, uint16_t kindShade) {

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

static GLuint linkShader(const std::vector<GLuint>& vecShaderID) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint ProgramID = glCreateProgram();
    for (auto shader : vecShaderID)
        glAttachShader(ProgramID, shader);

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

        throw Exception(std::string("link Shader Fail"));
    }

    for (auto id : vecShaderID)
        glDeleteShader(id);

    for (auto id : vecShaderID) {
        glDetachShader(ProgramID, id);
    }

    return ProgramID;
}

static std::unordered_map<GLenum, std::string> preProcess(const std::string source) {

    std::unordered_map<GLenum, std::string> shaderSource;

    const char* typeToken = "#type";
    size_t typeTokenLenght = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos) {

        size_t eol = source.find_first_of("\r\n", pos);
        // log erro
        if (eol == std::string::npos) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Syntax error");
            throw Exception(std::string("Syntax error ") + source);
        }

        size_t begin = pos + typeTokenLenght + 1;
        std::string type = source.substr(begin, eol - begin);
        GLenum t = shaderTypeFromString(type);

        if (t == 0) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Invalid Shader");
            throw Exception(std::string("Invalid Shader ") + source);
        }

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        shaderSource[t] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }

    return shaderSource;
}

static GLuint shadeLoadProg(const char* progName, const char* fileVertex, const char* fileFrag) {

    std::vector<GLuint> vecShaderID;
    vecShaderID.push_back(compileShader(utilReadFile(fileVertex), GL_VERTEX_SHADER));
    vecShaderID.push_back(compileShader(utilReadFile(fileFrag), GL_FRAGMENT_SHADER));

    GLuint shaderId = linkShader(vecShaderID); // Link o programa

    vecShaderID.clear();

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader %s id: %d", progName, shaderId);
    return shaderId;
}

static GLuint shadeLoadProg(const std::string& filepath) {

    std::vector<GLuint> vecShaderID;
    std::string fileData = utilReadFile(filepath);
    if (fileData.size() == 0)
        return 0;

    auto shaderSources = preProcess(fileData); // split shders kind
    for (auto& kv : shaderSources) {
        GLenum type = kv.first;
        const std::string source = kv.second;
        vecShaderID.push_back(compileShader(source, type)); // compile chade
    }

    GLuint shaderId = linkShader(vecShaderID); // Link o programa

    vecShaderID.clear();

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader %s id: %d", filepath.c_str(), (int)shaderId);
    return shaderId;
}

//----
Shader::Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath) : name(name) {
    this->shaderId = shadeLoadProg(name.c_str(), vertPath.c_str(), fragPath.c_str());
}

Shader::Shader(const std::string& filepath) {
    this->shaderId = shadeLoadProg(filepath);
    name = extractNameByFile(filepath);
}

Shader::~Shader() { glDeleteProgram(shaderId); }

GLint Shader::getUniform(const char* _varName) const noexcept {
    GLint loc = glGetUniformLocation(shaderId, _varName);
    if (loc == -1)
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%s\"", _varName, name.c_str());

    return loc;
}
} // namespace Chimera
