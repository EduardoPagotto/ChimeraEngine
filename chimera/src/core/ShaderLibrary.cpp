#include "chimera/core/ShaderLibrary.hpp"
#include "chimera/core/io/utils.hpp"

namespace Chimera {

bool ShaderLibrary::add(Shader* shader) {
    auto it = shaders.find(shader->getName());
    if (it == shaders.end()) {
        shaders[shader->getName()] = shader;
        return true;
    }

    return false;
}

Shader* ShaderLibrary::load(const std::string& filepath) {

    Shader* shader = this->get(extractNameByFile(filepath));
    if (shader != nullptr)
        return shader;

    shader = new Shader(filepath);
    shaders[shader->getName()] = shader;

    return shader;
}

Shader* ShaderLibrary::get(const std::string& name) {

    auto it = shaders.find(name);
    if (it != shaders.end())
        return it->second;

    return nullptr;
}
} // namespace Chimera