#include "Draw.h"
#include "NodeVisitor.h"

namespace Chimera {

Draw::Draw (Node *_parent, DrawType _type, std::string _name ) : Node (_parent, EntityKind::DRAW, _name ), type ( _type ) {
    
    pState = new State();
	pState->setEnableLight(Chimera::LightNum::LIGHTING, true);
	pState->setEnableColorMaterial(Chimera::ColorMaterial::COLOR_MATERIAL, false);

}

Draw::Draw ( const Draw &_draw ) : Node( _draw ) { 
}

Draw::~Draw() {
}

void Draw::init() {

}

void Draw::accept(NodeVisitor * v) {
	v->visit(this);
}

}
