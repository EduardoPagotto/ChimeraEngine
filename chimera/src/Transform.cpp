#include "Transform.hpp"
#include "NodeVisitor.hpp"
#include "OpenGLDefs.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Transform::Transform(Node* _parent, std::string _name)
    : Coord(_parent, EntityKind::TRANSFORM, _name) {

    model = glm::mat4(1.0f);
    staticTranslation = false;
}

Transform::~Transform() {}

void Transform::accept(NodeVisitor* v) { v->visit(this); }

glm::vec3 Transform::getPosition() { return glm::vec3(model[3]); }

void Transform::setPosition(const glm::vec3& _pos) {

    model = glm::translate(model, _pos);
}

glm::vec3 Transform::getRotation() { return glm::vec3(0.0, 0.0, 0.0); }

void Transform::setRotation(const glm::vec3& _rotation) {

    model = glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z);
}

glm::mat4 Transform::getMatrix() { return model; }

glm::mat4 Transform::getModelMatrix(Coord* _pCoord) { // ajuste matricial

    if (staticTranslation == false) {

        glm::mat4 matrixCoord = model;

        float* matrix = glm::value_ptr(matrixCoord);

        // pega posicao do objeto horigem de desenho (camera travada)
        glm::vec3 l_vec = _pCoord->getPosition();

        // desloca desenha para o pbjeto horigem
        matrix[12] -= l_vec.x;
        matrix[13] -= l_vec.y;
        matrix[14] -= l_vec.z;

        return glm::make_mat4(matrix);
    }

    return model;
}

void Transform::setMatrix(const glm::mat4& _trans) { model = _trans; }

void Transform::setPositionRotation(const glm::vec3& _posicao,
                                    const glm::vec3& _rotation) {

    this->setPosition(_posicao);
    this->setRotation(_rotation);
}
} // namespace Chimera
