#include "chimera/core/visible/Shader.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/utils.hpp"

namespace Chimera {

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

Shader::~Shader() noexcept {
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
} // namespace Chimera
