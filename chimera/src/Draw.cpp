#include "Draw.h"

namespace Chimera {

Draw::Draw (Node *_parent, EntityKind _kind, std::string _name ) : Node (_parent, _kind, _name ) {
   
	material = nullptr;
	shadersManager = Singleton<ShadersManager >::getRefSingleton();
}

Draw::Draw ( const Draw &_draw ) : Node( _draw ) { 

	material = _draw.material;
	shadersManager = Singleton<ShadersManager >::getRefSingleton();
}

Draw::~Draw() {

	material = nullptr;
	Singleton<ShadersManager >::releaseRefSingleton();
}
}
