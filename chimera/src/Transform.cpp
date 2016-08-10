//#include <vr/Vec3.h>
//#include <vr/Vec4.h>

#include "NodeVisitor.h"
#include "Transform.h"
#include "OpenGLDefs.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Chimera {

Transform::Transform(Node* _parent, std::string _name) : Coord (_parent, EntityKind::TRANSFORM, _name) {

    model = glm::mat4(1.0f);
    
}

Transform::~Transform() {
}

void Transform::accept(class NodeVisitor* v) {
	v->visit(this);
}

glm::vec3 Transform::getPosition() {
    
	return glm::vec3(model[3]);
    
}

void Transform::setPosition(const glm::vec3 & _pos) {
    
    model = glm::translate(model, _pos);
    
}

glm::vec3 Transform::getRotation() {
    
    return glm::vec3(0.0, 0.0, 0.0);
    
}

void Transform::setRotation ( const glm::vec3& _rotation ) {
    
    model = glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z); 
    
}

glm::mat4 Transform::getMatrix() {
    
    return model;
    
}

void Transform::setMatrix ( const glm::mat4& _trans ) {
    
    model = _trans;
    
}

void Transform::setPositionRotation ( const glm::vec3& _posicao, const glm::vec3& _rotation ) {
    
    this->setPosition(_posicao);
    this->setRotation(_rotation);
    
}
}
