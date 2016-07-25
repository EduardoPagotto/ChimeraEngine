#include "SceneRoot.h"
#include "NodeVisitor.h"

namespace Chimera {

SceneRoot::SceneRoot(Node * _pNode, std::string _name) : Node(_pNode, EntityKind::SCENEROOT, _name) {
	pState = new State();
}

SceneRoot::~SceneRoot() {
}

void SceneRoot::accept(NodeVisitor* v) {
	v->visit(this);
}

void SceneRoot::init() {
 
}

void SceneRoot::draw(Group *_pGroup) {
    
	pState->apply();
	pState->appyLighting();

    Chimera::DataMsg dataMsg ( KindOp::DRAW, this, _pGroup, nullptr );
    Node::update ( &dataMsg );   


}

void SceneRoot::drawHud(Group *_pGroup) {

	Chimera::DataMsg dataMsg(KindOp::DRAW_HUD, this, _pGroup, nullptr);
	Node::update(&dataMsg);

}


}
