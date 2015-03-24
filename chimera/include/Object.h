#ifndef OBJECT_H
#define OBJECT_H

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "Draw.h"
#include "Physics.h"

namespace Chimera {

	class Object : public Node {
	public:

		friend class Physics;

		Object(std::string _id, std::string _name);
		Object(const Object& _object);

		virtual ~Object();
		virtual void update(DataMsg *_dataMsg);

		virtual void clone(Node **ppNode);

		void init();

		void applyTorc(const btVector3 &_vet);
		void applyForce(const btVector3 &_vet);

		void setPositionRotation(const btVector3 &_posicao, const btVector3 &_rotation);

		btVector3& getPosition();

		void setTransform(const btTransform &_trans) {
			transform = _trans;
		}

		Draw * getDraw() {
			return  (Draw*)findChildByKind(EntityKind::DRAW, 0);
		}

	private:
		Physics *pPhysic;
		Draw *pDraw;

		btTransform transform;
	};

}

#endif
