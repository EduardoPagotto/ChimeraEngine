#include "Constraint.h"

namespace Chimera {

	Constraint::Constraint(std::string _id, std::string _name) : Node(EntityKind::CONSTRAINT, _id, _name) {

	}

	Constraint::Constraint(const Constraint& _constraint) : Node(_constraint) {

	}

	Constraint::~Constraint() {

	}

	void Constraint::clone(Node **ppNode) {
		*ppNode = new Constraint(*this);
		Node::clone(ppNode);
	}

	void Constraint::update(DataMsg *_dataMsg) {

		Node::update(_dataMsg);

	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
