#include "chimera/render/Transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Chimera {

Transform::Transform() : model(glm::mat4(1.0f)) {}

Transform::Transform(const glm::mat4& _trans) : model(_trans) {}

Transform::~Transform() {}

glm::vec3 Transform::getPosition() { return glm::vec3(model[3]); }

void Transform::setPosition(const glm::vec3& _pos) { model = glm::translate(model, _pos); }

glm::vec3 Transform::getRotation() { return glm::vec3(0.0, 0.0, 0.0); }

void Transform::setRotation(const glm::vec3& _rotation) {
    model = glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z);
}

glm::mat4 Transform::getMatrix() { return model; }

void Transform::setMatrix(const glm::mat4& _trans) { model = _trans; }

glm::mat4 Transform::getModelMatrix(const glm::vec3& _position) { // ajuste matricial
    glm::mat4 matrixCoord = model;
    float* matrix = glm::value_ptr(matrixCoord);
    // pega posicao do objeto horigem de desenho (viewpoint fixo)
    // desloca desenha para o pbjeto horigem
    matrix[12] -= _position.x;
    matrix[13] -= _position.y;
    matrix[14] -= _position.z;
    return glm::make_mat4(matrix);
}

// void Transform::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
//     glm::quat myQuat(_rotation);            // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
//     glm::mat4 matRot = glm::toMat4(myQuat); // matriz rotacao
//     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); // matriz translacao
//     transform = matRot * matTrans; // primeiro translada depois rotaciona, ordem é importante!!!
// }

void Transform::init(const glm::vec3& _size) {}

} // namespace Chimera
