#include "Group.hpp"

namespace Chimera {

Group::Group(Node* _parent, std::string _name) : Node(_parent, EntityKind::GROUP, _name) {
    shadoMapVisitor = nullptr;
    pShader = nullptr;
}

Group::~Group() {

    if (pShader != nullptr)
        delete pShader;
}

void Group::accept(NodeVisitor* v) { v->visit(this); }

void Group::init() {

    if (shadoMapVisitor != nullptr) {
        shadoMapVisitor->init();
    }
}

ShadowMap* Group::createSceneShadoMap(Coord* _pCoord) {

    if (shadoMapVisitor != nullptr) {
        shadoMapVisitor->pCoord = _pCoord;
        return shadoMapVisitor->render(this);
    }

    return nullptr;
}
} // namespace Chimera
