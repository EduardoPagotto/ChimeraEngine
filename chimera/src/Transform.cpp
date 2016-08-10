//#include <vr/Vec3.h>
//#include <vr/Vec4.h>

#include "NodeVisitor.h"
#include "Transform.h"
#include "OpenGLDefs.h"

namespace Chimera {

Transform::Transform(Node* _parent, std::string _name) : Coord (_parent, EntityKind::TRANSFORM, _name) {

	this->tflag = false;
	this->rflag = false;
	this->sflag = false;
}

Transform::~Transform() {
}

void Transform::accept(class NodeVisitor* v) {
	v->visit(this);
}

void Transform::setTransform ( const glm::mat4& _trans ) {
    //TODO implementar
}

glm::vec3 Transform::getPosition()
{
	return translate;
}

void Transform::setPosition(const glm::vec3 & _pos)
{
    this->tflag = true;
	translate = _pos;
}

bool Transform::getTranslateFlag() {
	return this->tflag;
}

void Transform::setRotate(const glm::vec3 &_vet) {
	this->rotate = _vet;
	this->rflag = true;
}

glm::vec3 Transform::getRotate() {
	return this->rotate;
}

bool Transform::getRotateFlag() {
	return this->rflag;
}

void Transform::setScale(const glm::vec3 &_vet) {
	this->scale = _vet;
	this->sflag = true;
}

glm::vec3 Transform::getScale() {
	return this->scale;
}

bool Transform::getScaleFlag() {
	return this->sflag;
}

void Transform::applyTransform()
{
	if (this->getTranslateFlag()) 
		glTranslatef(this->translate.x, this->translate.y, this->translate.z);

	if (this->getRotateFlag()) 
		glRotatef(0.0, this->rotate.x, this->rotate.y, this->rotate.z);

	if (this->getScaleFlag()) 
		glScalef(this->scale.x, this->scale.y, this->scale.z);
}

}
