#ifndef __SHADER_LOADER__HPP
#define __SHADER_LOADER__HPP

#include <string.h>

#include "chimera/core/Logger.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Shader.hpp"

namespace ChimeraLoaders {

class ShadersLoader {
  public:
    ShadersLoader();
    virtual ~ShadersLoader() {}
    GLuint load(const std::string& programName, const std::string& vertex_file_path,
                const std::string& fragment_file_path);
    Chimera::Shader* loadShader(const std::string& programName, const std::string& vertex_file_path,
                                const std::string& fragment_file_path);

  private:
    std::string getShaderCode(const char* file_path);
    GLuint compileShader(const std::string& shaderCode, bool _shadeKind);
    GLuint linkShader(const GLuint& VertexShaderID, const GLuint& FragmentShaderID);

    Chimera::Logger* log;
};
} // namespace ChimeraLoaders

#endif
