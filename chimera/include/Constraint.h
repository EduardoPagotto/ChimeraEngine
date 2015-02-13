#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <LinearMath/btTransform.h>

#include "Node.h"
#include "Physics.h"

namespace Chimera {

	class Constraint : public Node {
	public:

		friend class Loader;

		Constraint(std::string _id, std::string _name);
		Constraint(const Constraint& _constraint);
		virtual ~Constraint();

		virtual void clone(Node **ppNode);

		virtual void update(DataMsg *dataMsg);

	private:

		Physics *pPhysicsA;
		Physics *pPhysicsB;

		// btRigidBody* pRigidBodyA;
		// btRigidBody* pRigidBodyB;

		btTransform transformA;
		btTransform transformB;
	};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
