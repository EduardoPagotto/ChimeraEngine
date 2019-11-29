#include "chimera/core/LightData.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

LightData::LightData() {
    type = LightType::POSITIONAL;
    ambient = glm::vec4(0.0f);
    specular = glm::vec4(0.0f);
    diffuse = glm::vec4(0.0f);
    transform = glm::mat4(1.0); // Matrix identidade
}

LightData::~LightData() {}

void LightData::apply(Shader* _pShader) {
    glm::vec3 val = glm::vec3(transform[3]); // pega posicao
    _pShader->setGlUniform3fv("light.position", 1, glm::value_ptr(val));
    _pShader->setGlUniform4fv("light.ambient", 1, glm::value_ptr(ambient));
    _pShader->setGlUniform4fv("light.diffuse", 1, glm::value_ptr(diffuse));
    _pShader->setGlUniform4fv("light.specular", 1, glm::value_ptr(specular));
}
} // namespace Chimera