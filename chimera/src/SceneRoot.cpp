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
 
	Node::init();
}

void SceneRoot::draw(Object *pObjeto) {
    
	pState->apply();
	pState->appyLighting();

    Chimera::DataMsg dataMsg ( KindOp::DRAW, this, pObjeto, nullptr );
    Node::update ( &dataMsg );   
}

}
