#include "chimera_core/gl/Shader.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp> // Cabeçalho necessário

namespace ce {

    void Shader::setUniformU(const char* name, const Uniform& uv) noexcept {

        int32_t loc = getUniform(name);
        // SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Uniform: %s", name);
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

        // while (true) {
        //     // FIXME: o que ha de errado aqui ??
        //     GLenum erro = glGetError();
        //     if (erro != GL_NO_ERROR) {
        //         glm::mat4* aa = static_cast<glm::mat4*>(uv.ptr.get());
        //         // std::cout << glm::to_string(*aa) << std::endl;
        //         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Uniform OpenGL Erro: %s Code %d -> %s", name, erro,
        //                      glm::to_string(*aa).c_str());
        //     } else {
        //         break;
        //     }
        // }
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
} // namespace ce
