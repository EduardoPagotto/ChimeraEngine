#include "Camera.hpp"
#include "NodeVisitor.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Camera::Camera(Node* _pNode, CameraType _type, std::string _name)
    : type(_type), Node(_pNode, EntityKind::CAMERA, _name) {

    type = _type;
    position = glm::vec3(0.0, 0.0, 0.0);  // setZero();
    rotation = glm::vec3(0.0, 0.0, 0.0);  //.setZero();
    direction = glm::vec3(0.0, 0.0, 0.0); //.setZero();
    nearDistance = 0.1f;
    farDistance = 1000.0f;
    fov = 45.0f;
    perspective = true;
}

Camera::~Camera() {}

void Camera::setTransform(const glm::mat4& _trans) {
    position = glm::vec3(_trans[3]);
    direction = glm::vec3(0.0, 0.0, 0.0); // FIXME encontrar na matrix _trans
    rotation = glm::vec3(0.0, 0.0, -1.0); // FIXME pegar no arquivo do collada
}

void Camera::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {

    position = _posicao;
    direction = _rotation;                // FIXME: esta errado aqui
    rotation = glm::vec3(0.0, 0.0, -1.0); // FIXME pegar no arquivo do collada
}

glm::mat4 Camera::getViewMatrix(void) {

    return glm::lookAt(position, direction, rotation);
}

void Camera::accept(NodeVisitor* v) { v->visit(this); }
} // namespace Chimera
