#include "chimera/node/Light.hpp"
#include "chimera/node/NodeVisitor.hpp"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Light::Light(Node* _parent, std::string _name) : Node(_parent, EntityKind::LIGHT, _name) {
    number = 0;
    type = LightType::POSITIONAL;
    ambient = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    transform = glm::mat4(1.0); // Matrix identidade
}

Light::~Light() {}

void Light::accept(NodeVisitor* v) { v->visit(this); }

void Light::apply(Shader* _pShader) {
    glm::vec3 val = glm::vec3(transform[3]); // pega posicao
    _pShader->setGlUniform3fv("light.position", 1, glm::value_ptr(val));
    _pShader->setGlUniform4fv("light.ambient", 1, glm::value_ptr(ambient));
    _pShader->setGlUniform4fv("light.diffuse", 1, glm::value_ptr(diffuse));
    _pShader->setGlUniform4fv("light.specular", 1, glm::value_ptr(specular));
}

// void Light::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
//     glm::quat myQuat(_rotation);            // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
//     glm::mat4 matRot = glm::toMat4(myQuat); // matriz rotacao
//     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); // matriz translacao
//     transform = matRot * matTrans; // primeiro translada depois rotaciona, ordem é importante!!!
// }

void Light::init() {
    // position = glm::vec3(transform[3]); // position = transform.getOrigin();
}
} // namespace Chimera