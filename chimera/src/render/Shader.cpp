#include "chimera/render/Shader.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/utils.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

static GLuint compileShader(const std::string& fileName, const std::string& shaderCode, uint16_t kindShade) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderID = 0;
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
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Shader %s compile error: %s", fileName.c_str(),
                         std::string(&shaderErrorMessage[0]).c_str());

            throw Exception(std::string("Shader compile fail: ") + shaderCode);
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

//----

void Shader::invalidade() {
    glDeleteProgram(progID);
    progID = 0;
}

const GLint Shader::getUniform(const char* _varName) const noexcept {
    GLint loc = glGetUniformLocation(progID, _varName);
    if (loc == -1)
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%d\"", _varName, progID);

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
std::unordered_map<std::string, Shader> ShaderManager::mShaders;

void ShaderManager::load(const std::string& name, const std::unordered_map<GLenum, std::string>& mFiles, Shader& shader) {

    Shader s = ShaderManager::get(name);
    if (s.isInvalid()) {

        std::vector<GLuint> vecShaderID;
        for (auto& kv : mFiles) {
            std::string source;
            utilsReadFile(kv.second, source);
            vecShaderID.push_back(compileShader(name, source, kv.first)); // compile shader
        }

        shader.progID = linkShader(vecShaderID); // Link o programa
        vecShaderID.clear();
        ShaderManager::mShaders[name] = shader;

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "New Shader %s id: %d", name.c_str(), (int)shader.progID);
    } else {
        shader.progID = s.progID;
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Dup Shader %s id: %d", name.c_str(), (int)shader.progID);
    }
}

const Shader ShaderManager::get(const std::string& name) {

    std::unordered_map<std::string, Shader>::const_iterator got = ShaderManager::mShaders.find(name);
    if (got != ShaderManager::mShaders.end())
        return got->second;

    return Shader();
}

bool ShaderManager::remove(const std::string& name) {

    std::unordered_map<std::string, Shader>::iterator got = ShaderManager::mShaders.find(name);
    if (got != ShaderManager::mShaders.end()) {

        got->second.invalidade();
        ShaderManager::mShaders.erase(got);
        return true;
    }

    return false;
}

void ShaderManager::clear() {
    for (auto it = ShaderManager::mShaders.begin(); it != ShaderManager::mShaders.end(); it++)
        it->second.invalidade();

    ShaderManager::mShaders.clear();
}
} // namespace Chimera
