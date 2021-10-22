#include "chimera/render/Light.hpp"

namespace Chimera {

Light::Light() { type = LightType::POSITIONAL; }

Light::~Light() {}

void Light::bindLightInformation(Shader* _pShader) {
    for (const UniformVal& uniformLight : listProp)
        uniformLight.setUniform(_pShader);
}

void Light::bindLightInformation(std::vector<UniformVal>& uniforms) {
    for (const UniformVal& uniformLight : listProp)
        uniforms.push_back(uniformLight);
}
} // namespace Chimera