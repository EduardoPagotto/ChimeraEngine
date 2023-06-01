#include "chimera/core/visible/Shader.hpp"
#include "chimera/core/utils.hpp"
#include <SDL2/SDL.h>
#include <vector>

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

            throw std::string("Shader compile fail: " + shaderCode);
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

        throw std::string("link Shader Fail");
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
    uniformLocationCache.clear();
}

const GLint Shader::getUniform(const std::string& name) const noexcept {

    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    GLint loc = glGetUniformLocation(progID, name.c_str());
    uniformLocationCache[name] = loc;

    return loc;
}

void Shader::setUniformU(const char* name, const UValue& uv) {

    GLint loc = getUniform(name);
    if (loc == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%d\"", name, progID);
        return;
    }

    switch (uv.type) {
        case UniformType::INT:
            glUniform1i(loc, uv.u.vint);
            break;
        case UniformType::FLOAT:
            glUniform1f(loc, uv.u.vfloat);
            break;
        case UniformType::VEC2:
            glUniform2fv(loc, 1, glm::value_ptr(uv.u.vvec2));
            break;
        case UniformType::IVEC2:
            glUniform2iv(loc, 1, glm::value_ptr(uv.u.vivec2));
            break;
        case UniformType::VEC3:
            glUniform3fv(loc, 1, glm::value_ptr(uv.u.vvec3));
            break;
        case UniformType::IVEC3:
            glUniform3iv(loc, 1, glm::value_ptr(uv.u.vivec3));
            break;
        case UniformType::VEC4:
            glUniform4fv(loc, 1, glm::value_ptr(uv.u.vvec4));
            break;
        case UniformType::IVEC4:
            glUniform4iv(loc, 1, glm::value_ptr(uv.u.vivec4));
            break;
        case UniformType::MAT3:
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(uv.u.vmat3));
            break;
        case UniformType::MAT4:
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(uv.u.vmat4));
            break;
        case UniformType::INTP:
            glUniform1iv(loc, uv.size, uv.u.pint);
            break;

        case UniformType::INVALID:
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Uniform \"%s\" invalid in Program \"%d\"", name, progID);
            break;
    }
}

//---

void ShaderManager::load(const std::string& name, const std::unordered_map<GLenum, std::string>& mFiles, Shader& shader) {

    std::unordered_map<std::string, Shader>::const_iterator got = ShaderManager::mShaders.find(name);
    if (got == ShaderManager::mShaders.end()) {

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
        shader = got->second;
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Dup Shader %s id: %d", name.c_str(), (int)shader.progID);
    }
}

const Shader& ShaderManager::get(const std::string& name) {

    std::unordered_map<std::string, Shader>::const_iterator got = ShaderManager::mShaders.find(name);
    if (got != ShaderManager::mShaders.end())
        return got->second;

    throw std::string("Shader nao existe: " + name);
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
