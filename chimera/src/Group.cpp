#include "Group.h"
#include "NodeVisitor.h"

namespace Chimera {

Group::Group(Node* _parent, std::string _name) : Node (_parent, EntityKind::GROUP, _name) {
}

Group::~Group() {
}

void Group::accept(NodeVisitor* v){
    v->visit(this);
}

void Group::init() {
}
}


