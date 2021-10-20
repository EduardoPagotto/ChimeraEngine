#include "chimera/render/Light.hpp"

namespace Chimera {

Light::Light() {
    type = LightType::POSITIONAL;
    ambient = glm::vec4(0.0f);
    specular = glm::vec4(0.0f);
    diffuse = glm::vec4(0.0f);
    transform = glm::mat4(1.0); // Matrix identidade
}

Light::~Light() {}

void Light::bindLightInformation(Shader* _pShader) {
    glm::vec3 val = glm::vec3(transform[3]); // pega posicao
    _pShader->setUniform(SHADE_LIGHT_POSITION, val);
    _pShader->setUniform(SHADE_LIGHT_AMBIENT, ambient);
    _pShader->setUniform(SHADE_LIGHT_DIFFUSE, diffuse);
    _pShader->setUniform(SHADE_LIGHT_SPECULAR, specular);
}
} // namespace Chimera