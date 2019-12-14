#include "chimera/node/NodeGroup.hpp"

namespace Chimera {

NodeGroup::NodeGroup(Node* _parent, std::string _name) : Node(_parent, Kind::GROUP, _name) {
    shadoMapVisitor = nullptr;
    pShader = nullptr;
}

NodeGroup::~NodeGroup() {

    if (pShader != nullptr)
        delete pShader;
}

void NodeGroup::accept(VisitorInterface* v) { v->visit(this); }

void NodeGroup::init() {
    if (shadoMapVisitor != nullptr) {
        shadoMapVisitor->init();
    }
}
} // namespace Chimera
