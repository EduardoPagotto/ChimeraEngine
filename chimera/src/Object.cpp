#include "Object.h"

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>

namespace Chimera {

	Object::Object(std::string _id, std::string _name) : Node(EntityKind::OBJECT, _id, _name) {

		pPhysic = nullptr;
		pDraw = nullptr;
		pMaterial = nullptr;

		transform.setIdentity();

	}

	Object::Object(const Object& _object) : Node(_object) {

		pPhysic = _object.pPhysic;
		pDraw = _object.pDraw;
		pMaterial = _object.pMaterial;

		transform = _object.transform;

	}

	Object::~Object() {
	}

	void Object::setPositionRotation(const btVector3 &_posicao, const btVector3 &_rotation) {

		//Transformacao quando Euley nao apagar
		btQuaternion l_qtn;
		transform.setIdentity();
		l_qtn.setEulerZYX(_rotation.getX(), _rotation.getY(), _rotation.getZ());
		transform.setRotation(l_qtn);
		transform.setOrigin(_posicao);
		//pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

	}

	void Object::init() {

		pMaterial = (Material*)findChildByKind(EntityKind::MATERIAL, 0);
		if (pMaterial == nullptr) {
			pMaterial = new Material(getId() + "_Material_defaultCreate", getName() + "_Material_default_create");
			pMaterial->createDefaultEffect();
		}

		pDraw = (Draw*)findChildByKind(EntityKind::DRAW, 0);

		pPhysic = (Physics*)findChildByKind(EntityKind::PHYSICS, 0);
		if (pPhysic == nullptr) {

			//Cria corpo caso nao exista
			pPhysic = new Physics("", "");
			pPhysic->setMass(0.5f);
			pPhysic->setFriction(0.0f);
			pPhysic->setRestitution(0.0f);

			if (pDraw != nullptr)
				pPhysic->setShapeBox(pDraw->getSizeBox());
		}

		if (pPhysic->isShapeDefine()==false)
			pPhysic->setShapeBox(pDraw->getSizeBox());

		pPhysic->init(transform, this);

	}

	void Object::render() {

		pMaterial->render();

		if (pDraw != nullptr)
			pDraw->render();

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void Object::clone(Node **ppNode) {
		*ppNode = new Object(*this);
		Node::clone(ppNode);
	}

	void Object::update(DataMsg *_dataMsg) {

		if (_dataMsg->getKindOp() == KindOp::START) {

			//inicialize primeiro os filhos para garantir textura e efeito em material
			Node::update(_dataMsg);

			//inicializa objeto local
			init();

		}
		if (_dataMsg->getKindOp() == KindOp::DRAW3D) {

			glPushMatrix();

			if (pPhysic != nullptr) {
				Object *pSource = (Object *)_dataMsg->getParam();
				pPhysic->ajusteMatrix(pSource->pPhysic);
			}

			render();

			Node::update(_dataMsg);

			glPopMatrix();
		}
		if (_dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE) {

			_dataMsg->setDone(true);

		}
	}

	btVector3& Object::getPosition() {
		return pPhysic->getPosition();
	}

	void Object::applyTorc(const btVector3 &_vet) {
		if (pPhysic)
			pPhysic->torque(_vet);
	}

	void Object::applyForce(const btVector3 &_vet) {
		if (pPhysic)
			pPhysic->propulcao(_vet);
	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
