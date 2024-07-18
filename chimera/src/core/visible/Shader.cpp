#include "chimera/core/visible/Shader.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/utils.hpp"

namespace Chimera {

static uint32_t compileShader(const std::string& fileName, const std::string& shaderCode, uint16_t kindShade) {

    int32_t Result = GL_FALSE;
    int InfoLogLength;

    uint32_t shaderID = 0;
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

static uint32_t linkShader(const std::vector<uint32_t>& vecShaderID) {

    int32_t Result = GL_FALSE;
    int InfoLogLength;

    uint32_t ProgramID = glCreateProgram();
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

void Shader::setUniformU(const char* name, const UValue& uv) noexcept {

    int32_t loc = getUniform(name);
    if (loc == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader Uniform \"%s\" not found in Program \"%d\"", name, progID);
        return;
    }

    switch (uv.type) {
        // case UniformType::UINT:
        //     glUniform1uiv(loc, 1, static_cast<uint*>(uv.ptr.get()));
        //     break;
        case UniformType::INT:
            glUniform1iv(loc, 1, static_cast<int*>(uv.ptr.get()));
            break;
        case UniformType::INT_VEC: {
            auto lst = static_cast<std::vector<int>*>(uv.ptr.get());
            glUniform1iv(loc, lst->size(), &(*lst)[0]);
        } break;
        case UniformType::IVEC2: // glUniform2iv(loc, 1, glm::value_ptr(uv.u.vivec2));
            glUniform2iv(loc, 1, static_cast<int*>(uv.ptr.get()));
            break;
        case UniformType::IVEC3:
            glUniform3iv(loc, 1, static_cast<int*>(uv.ptr.get()));
            break;
        case UniformType::IVEC4:
            glUniform4iv(loc, 1, static_cast<int*>(uv.ptr.get()));
            break;
        case UniformType::FLOAT:
            glUniform1fv(loc, 1, static_cast<float*>(uv.ptr.get()));
            break;
        case UniformType::FLOAT_VEC: {
            auto lst = static_cast<std::vector<float>*>(uv.ptr.get());
            glUniform1fv(loc, lst->size(), &(*lst)[0]);
        } break;
        case UniformType::VEC2:
            glUniform2fv(loc, 1, static_cast<float*>(uv.ptr.get()));
            break;
        case UniformType::VEC3:
            glUniform3fv(loc, 1, static_cast<float*>(uv.ptr.get()));
            break;
        case UniformType::VEC4:
            glUniform4fv(loc, 1, static_cast<float*>(uv.ptr.get()));
            break;
        case UniformType::MAT3:
            glUniformMatrix3fv(loc, 1, GL_FALSE, static_cast<float*>(uv.ptr.get()));
            break;
        case UniformType::MAT4:
            glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<float*>(uv.ptr.get()));
            break;
        case UniformType::INVALID:
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Uniform \"%s\" invalid in Program \"%d\"", name, progID);
            break;
    }
}

void Shader::invalidade() noexcept {
    glDeleteProgram(progID);
    progID = 0;
    uniformLocationCache.clear();
}

const int32_t Shader::getUniform(const std::string& name) const noexcept {

    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    int32_t loc = glGetUniformLocation(progID, name.c_str());
    uniformLocationCache[name] = loc;

    return loc;
}

//---

void ShaderManager::load(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles, Shader& shader) noexcept {

    std::unordered_map<std::string, Shader>::const_iterator got = ShaderManager::mShaders.find(name);
    if (got == ShaderManager::mShaders.end()) {

        std::vector<uint32_t> vecShaderID;
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

bool ShaderManager::remove(const std::string& name) noexcept {

    std::unordered_map<std::string, Shader>::iterator got = ShaderManager::mShaders.find(name);
    if (got != ShaderManager::mShaders.end()) {

        got->second.invalidade();
        ShaderManager::mShaders.erase(got);
        return true;
    }

    return false;
}

void ShaderManager::clear() noexcept {
    for (auto it = ShaderManager::mShaders.begin(); it != ShaderManager::mShaders.end(); it++)
        it->second.invalidade();

    ShaderManager::mShaders.clear();
}
} // namespace Chimera
