#include "Draw.h"

namespace Chimera {

Draw::Draw (Node *_parent, EntityKind _kind, std::string _name ) : Node (_parent, _kind, _name ) {
    
	//pState = new State();
	//pState->setEnableLight(Chimera::LightNum::LIGHTING, true);
	//pState->setEnableColorMaterial(Chimera::ColorMaterial::COLOR_MATERIAL, false);

	material = nullptr;
}

Draw::Draw ( const Draw &_draw ) : Node( _draw ) { 
}

Draw::~Draw() {
}
}
