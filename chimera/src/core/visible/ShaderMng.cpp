#include "chimera/core/visible/ShaderMng.hpp"
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

std::shared_ptr<Shader> ShaderMng::load(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles) noexcept {

    auto got = ShaderMng::map_shaders.find(name);
    if (got == ShaderMng::map_shaders.end()) {

        std::vector<uint32_t> vecShaderID;
        for (auto& kv : mFiles) {
            std::string source;
            utilsReadFile(kv.second, source);
            vecShaderID.push_back(compileShader(name, source, kv.first)); // compile shader
        }

        std::shared_ptr<Shader> shader = std::make_shared<Shader>(linkShader(vecShaderID)); // Link o programa
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "New Shader %s id: %d", name.c_str(), (int)shader->getID());
        ShaderMng::map_shaders[name] = shader;

        vecShaderID.clear();
        return shader;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Dup Shader %s", name.c_str());
    return got->second;
}

void ShaderMng::remove(std::shared_ptr<Shader> shader) noexcept {
    for (auto it = ShaderMng::map_shaders.begin(); it != ShaderMng::map_shaders.end(); it++) {
        if (it->second == shader) {
            it->second = nullptr;
            ShaderMng::map_shaders.erase(it);
        }
    }
}

void ShaderMng::clear() noexcept {
    for (auto it = ShaderMng::map_shaders.begin(); it != ShaderMng::map_shaders.end(); it++)
        it->second = nullptr;

    ShaderMng::map_shaders.clear();
}
} // namespace Chimera