#include "chimera/render/Shader.hpp"
#include "chimera/render/Exception.hpp"
#include "chimera/render/io/utils.hpp"
#include <SDL2/SDL.h>

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

GLint Shader::getUniform(const char* _varName) const noexcept {
    GLint loc = glGetUniformLocation(shaderId, _varName);
    if (loc == -1)
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%s\"", _varName, name.c_str());

    return loc;
}

//---

void UniformVal::setUniform(const Shader& shader) const {
    switch (typeVal) {
        case 0:
            shader.setUniform(name.c_str(), val_int);
            break;
        case 1:
            shader.setUniform(name.c_str(), val_float);
            break;
        case 2:
            shader.setUniform(name.c_str(), val_vec2);
            break;
        case 3:
            shader.setUniform(name.c_str(), val_ivec2);
            break;
        case 4:
            shader.setUniform(name.c_str(), val_vec3);
            break;
        case 5:
            shader.setUniform(name.c_str(), val_ivec3);
            break;
        case 6:
            shader.setUniform(name.c_str(), val_vec4);
            break;
        case 7:
            shader.setUniform(name.c_str(), val_ivec4);
            break;
        case 8:
            shader.setUniform(name.c_str(), val_mat3);
            break;
        case 9:
            shader.setUniform(name.c_str(), val_mat4);
            break;
    }
}

//---
void UniformMapped::set(const UniformVal& value) { uniformMap[value.getName()] = value; }
void UniformMapped::bindAll(const Shader& shader) const {
    for (auto& kv : uniformMap)
        kv.second.setUniform(shader);
}
void UniformMapped::clear() { uniformMap.clear(); }

//---
std::vector<Shader> ShaderManager::shaders;

void ShaderManager::load(const std::string& filepath, Shader& shader) {

    if (ShaderManager::get(extractNameByFile(filepath), shader))
        return;

    shader.shaderId = shadeLoadProg(filepath);
    shader.name = extractNameByFile(filepath);

    ShaderManager::shaders.push_back(shader);
}

bool ShaderManager::get(const std::string& name, Shader& shader) {

    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        if (it->name == name) {
            shader.shaderId = it->shaderId;
            shader.name = it->name;
            return true;
        }
    }

    return false;
}

bool ShaderManager::remove(Shader& shader) {

    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        if (it->name == shader.name) {
            glDeleteProgram(shader.shaderId);
            shader.shaderId = 0;
            return true;
        }
    }
    return false;
}

void ShaderManager::clear() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        glDeleteProgram(it->shaderId);
        it->shaderId = 0;
    }

    shaders.clear();
}

} // namespace Chimera
