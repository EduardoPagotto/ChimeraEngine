#include "chimera/render/Light.hpp"

namespace Chimera {

Light::Light() { type = LightType::POSITIONAL; }

Light::~Light() {}

void Light::bindLightInformation(const Shader& shader) {
    for (const UniformVal& uniformLight : listProp)
        uniformLight.setUniform(shader);
}

void Light::bindLightInformation(std::vector<UniformVal>& uniforms) {
    for (const UniformVal& uniformLight : listProp)
        uniforms.push_back(uniformLight);
}
} // namespace Chimera