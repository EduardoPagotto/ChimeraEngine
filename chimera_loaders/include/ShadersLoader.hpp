#ifndef __SHADER_LOADER__HPP
#define __SHADER_LOADER__HPP

#include <string.h>

#include "OpenGLDefs.hpp"
#include "Shader.hpp"

#include <spdlog/spdlog.h>

namespace ChimeraLoaders {

class ShadersLoader {
  public:
    ShadersLoader();
    virtual ~ShadersLoader() {}
    GLuint load(const std::string& programName, const std::string& vertex_file_path,
                const std::string& fragment_file_path);
    Chimera::Shader* loadShader(const std::string& programName,
                                const std::string& vertex_file_path,
                                const std::string& fragment_file_path);

  private:
    std::string getShaderCode(const char* file_path);
    GLuint compileShader(const std::string& shaderCode, bool _shadeKind);
    GLuint linkShader(const GLuint& VertexShaderID, const GLuint& FragmentShaderID);

    std::shared_ptr<spdlog::logger> log;
};
} // namespace ChimeraLoaders

#endif
