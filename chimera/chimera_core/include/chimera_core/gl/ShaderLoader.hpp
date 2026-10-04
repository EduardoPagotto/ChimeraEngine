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

        result_type operator()(const std::unordered_map<uint32_t, std::string>& files) const {
            return ShaderLoader::load_from_files(files);
        }

      private:
        static uint32_t link_shader(const std::vector<uint32_t>& vec_shader_id) {

            int32_t result = GL_FALSE;
            int info_log_length;

            uint32_t program_id = glCreateProgram();
            for (auto shader : vec_shader_id) {
                glAttachShader(program_id, shader);
            }

            glLinkProgram(program_id);

            // Check the program
            glGetProgramiv(program_id, GL_LINK_STATUS, &result);

            if (result == GL_FALSE) {

                glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
                if (info_log_length > 0) {
                    std::vector<char> program_error_message(info_log_length + 1);
                    glGetProgramInfoLog(program_id, info_log_length, nullptr, program_error_message.data());

                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[ShaderLoader] Shader Check program: %s",
                                 std::string(program_error_message.data()).c_str());
                }

                throw std::runtime_error("[ShaderLoader] Link fail");
            }

            for (auto id : vec_shader_id) {
                glDeleteShader(id);
            }

            for (auto id : vec_shader_id) {
                glDetachShader(program_id, id);
            }

            return program_id;
        }

        static uint32_t compile_shader(const std::string& shader_code, uint16_t kind_shade) {

            int32_t result = GL_FALSE;
            int info_log_length;

            uint32_t shader_id = 0;
            shader_id = glCreateShader(kind_shade);

            char const* source_pointer = shader_code.c_str();
            glShaderSource(shader_id, 1, &source_pointer, NULL);
            glCompileShader(shader_id);

            // Check Fragment Shader
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

            if (result == GL_FALSE) {
                glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
                if (info_log_length > 0) {

                    std::vector<char> shader_error_message(info_log_length + 1);
                    glGetShaderInfoLog(shader_id, info_log_length, nullptr, shader_error_message.data());
                    throw std::runtime_error(std::format("[ShaderLoader] Compile fail: {}", shader_code).c_str());
                }
            }

            return shader_id;
        }

        static void read_file(const std::string& filepath, std::string& result) {
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

        static result_type load_from_files(const std::unordered_map<uint32_t, std::string>& files) {

            std::vector<uint32_t> vec_shader_id;
            for (const auto& kv : files) {
                std::string source;
                ShaderLoader::read_file(kv.second, source);
                vec_shader_id.push_back(ShaderLoader::compile_shader(source, kv.first)); // compile shader

                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[ShaderLoader] Load %s", kv.second.c_str());
            }

            std::shared_ptr<Shader> shader =
                std::make_shared<Shader>(ShaderLoader::link_shader(vec_shader_id)); // Link o programa

            vec_shader_id.clear();

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[ShaderLoader] Link %d", shader->get_id());

            return shader;
        }
    };

} // namespace ce
