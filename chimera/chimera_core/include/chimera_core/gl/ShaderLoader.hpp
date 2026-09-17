#pragma once

#include "chimera_core/gl/OpenGLDefs.hpp"
#include "chimera_core/gl/Shader.hpp"
#include <SDL3/SDL_log.h>
#include <format>
#include <fstream>
#include <stdexcept>
#include <string>

namespace ce {

    struct ShaderLoader {
        using result_type = std::shared_ptr<Shader>;

        result_type operator()(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles) const {
            return ShaderLoader::LoadFromFile(name, mFiles);
        }

      private:
        static uint32_t LinkShader(const std::vector<uint32_t>& vecShaderID) {

            int32_t Result = GL_FALSE;
            int InfoLogLength;

            uint32_t ProgramID = glCreateProgram();
            for (auto shader : vecShaderID) {
                glAttachShader(ProgramID, shader);
            }

            glLinkProgram(ProgramID);

            // Check the program
            glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);

            if (Result == GL_FALSE) {

                glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
                if (InfoLogLength > 0) {
                    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
                    glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, ProgramErrorMessage.data());

                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[ShaderLoader] Shader Check program: %s",
                                 std::string(ProgramErrorMessage.data()).c_str());
                }

                throw std::runtime_error("[ShaderLoader] Link fail");
            }

            for (auto id : vecShaderID) {
                glDeleteShader(id);
            }

            for (auto id : vecShaderID) {
                glDetachShader(ProgramID, id);
            }

            return ProgramID;
        }

        static uint32_t CompileShader(const std::string& fileName, const std::string& shaderCode, uint16_t kindShade) {

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
                    glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage.data());

                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[ShaderLoader] Shader %s compile error: %s",
                                 fileName.c_str(), std::string(shaderErrorMessage.data()).c_str());

                    throw std::runtime_error(std::format("[ShaderLoader] Shader compile fail: {}", shaderCode).c_str());
                }
            }

            return shaderID;
        }

        static void ReadFile(const std::string& filepath, std::string& result) {
            std::ifstream in(filepath, std::ios::in | std::ios::binary);
            if (in) {
                in.seekg(0, std::ios::end);
                result.resize(in.tellg());
                in.seekg(0, std::ios::beg);
                in.read(result.data(), static_cast<std::streamsize>(result.size()));
                in.close();
            } else {
                throw std::runtime_error(std::format("[ShaderLoader] File not found: {}", filepath).c_str());
            }
        }

        static result_type LoadFromFile(const std::string& name,
                                        const std::unordered_map<uint32_t, std::string>& mFiles) {

            std::vector<uint32_t> vecShaderID;
            for (const auto& kv : mFiles) {
                std::string source;
                ShaderLoader::ReadFile(kv.second, source);
                vecShaderID.push_back(ShaderLoader::CompileShader(name, source, kv.first)); // compile shader
            }

            std::shared_ptr<Shader> shader =
                std::make_shared<Shader>(ShaderLoader::LinkShader(vecShaderID)); // Link o programa

            vecShaderID.clear();

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[ShaderLoader] Load %s id: %d", name.c_str(),
                         (int)shader->getID());
            return shader;
        }
    };

} // namespace ce
