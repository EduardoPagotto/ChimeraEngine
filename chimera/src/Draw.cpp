#include "Draw.h"

namespace Chimera {

Draw::Draw (Node *_parent, EntityKind _kind, std::string _name ) : Node (_parent, _kind, _name ) {
   
	material = nullptr;
	shader = Singleton<Shader>::getRefSingleton();
}

Draw::Draw ( const Draw &_draw ) : Node( _draw ) { 

	material = _draw.material;
	shader = Singleton<Shader>::getRefSingleton();
}

Draw::~Draw() {

	material = nullptr;
	Singleton<Shader>::releaseRefSingleton();
}
}
