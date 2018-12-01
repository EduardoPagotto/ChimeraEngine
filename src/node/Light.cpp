#include "chimera/node/Light.hpp"
#include "chimera/node/NodeVisitor.hpp"

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Light::Light(Node* _parent, std::string _name) : Node(_parent, EntityKind::LIGHT, _name) {

    number = 0;
    type = LightType::POSITIONAL;
    ambient = Color::ZERO;
    specular = Color::ZERO;
    diffuse = Color::ZERO;
    position = glm::vec3(0.0, 0.0, 0.0);
    transform = glm::mat4(1.0); // transform.setIdentity();
}

Light::~Light() {}

void Light::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {

    //     //Transformacao quando Euley nao apagar
    //     btQuaternion l_qtn;
    //     transform.setIdentity();
    //     l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    //     transform.setRotation ( l_qtn );
    //     transform.setOrigin ( _posicao );
    //     //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
    //     l_posicao));

    glm::quat myQuat(_rotation);            // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
    glm::mat4 matRot = glm::toMat4(myQuat); // matriz rotacao
    glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); // matriz translacao
    transform = matRot * matTrans; // primeiro translada depois rotaciona, ordem é importante!!!
}

void Light::accept(NodeVisitor* v) { v->visit(this); }

void Light::init() {
    position = glm::vec3(transform[3]); // position = transform.getOrigin();
}

} // namespace Chimera