//#include <vr/Vec3.h>
//#include <vr/Vec4.h>

#include "NodeVisitor.h"
#include "Transform.h"
#include "OpenGLDefs.h"

namespace Chimera {

Transform::Transform(Node* _parent, std::string _name) : Group(_parent, _name) {

	this->setKind(EntityKind::TRANSFORM);
	this->tflag = false;
	this->rflag = false;
	this->sflag = false;
}

Transform::~Transform() {
}

void Transform::update(DataMsg * _dataMsg)
{
	if ((_dataMsg->getKindOp() == KindOp::DRAW) || (_dataMsg->getKindOp() == KindOp::DRAW_NO_TEX)) {

		this->applyTransform();

	}


	Group::update(_dataMsg);
}

void Transform::accept(class NodeVisitor* v) {
	v->visit(this);
}

void Transform::setTranslate(const btVector3 &_vet) {
	this->translate = _vet;
	this->tflag = true;
}

btVector3 Transform::getTranslate() {
	return this->translate;
}

bool Transform::getTranslateFlag() {
	return this->tflag;
}

void Transform::setRotate(const btVector3 &_vet) {
	this->rotate = _vet;
	this->rflag = true;
}

btVector3 Transform::getRotate() {
	return this->rotate;
}

bool Transform::getRotateFlag() {
	return this->rflag;
}

void Transform::setScale(const btVector3 &_vet) {
	this->scale = _vet;
	this->sflag = true;
}

btVector3 Transform::getScale() {
	return this->scale;
}

bool Transform::getScaleFlag() {
	return this->sflag;
}

void Transform::applyTransform()
{
	if (this->getTranslateFlag()) 
		glTranslatef(this->translate.x(), this->translate.y(), this->translate.z());

	if (this->getRotateFlag()) 
		glRotatef(this->rotate.x(), this->rotate.y(), this->rotate.z(), this->rotate.w());

	if (this->getScaleFlag()) 
		glScalef(this->scale.x(), this->scale.y(), this->scale.z());
}

}
