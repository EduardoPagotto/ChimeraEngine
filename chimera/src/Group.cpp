#include "Group.h"


namespace Chimera {

Group::Group(Node* _parent, std::string _name) : Node (_parent, EntityKind::GROUP, _name) {
	shadoMapVisitor = nullptr;
}

Group::~Group() {
}

void Group::accept(NodeVisitor* v){
    v->visit(this);
}

void Group::init() {
}

bool Group::executeShadoMap( Coord *_pCoord ) {

	if (shadoMapVisitor != nullptr) {
		shadoMapVisitor->pCoord = _pCoord;
		shadoMapVisitor->execute(this);
		return true;
	}

	return false;
}


}


