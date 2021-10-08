#ifndef __CHIMERA_SHADER_LIBRARY__HPP
#define __CHIMERA_SHADER_LIBRARY__HPP

#include "Shader.hpp"
#include <unordered_map>

namespace Chimera {

class ShaderLibrary {
  public:
    bool add(Shader* shader);
    Shader* load(const std::string& filepath);
    Shader* get(const std::string& name);

  private:
    std::unordered_map<std::string, Shader*> shaders;
};
} // namespace Chimera
#endif