#include "Entity.h"

namespace Chimera {

unsigned Entity::serialMaster = 0;

std::string Entity::getNameKindNode ( EntityKind _kind ) {

    switch ( _kind ) {
    case EntityKind::NODE:
        return std::string ( "NODE" );
        break;
    case EntityKind::CAMERA:
        return std::string ( "CAMERA" );
        break;
    case EntityKind::LIGHT:
        return std::string ( "LIGHT" );
        break;
    case EntityKind::OBJECT:
        return std::string ( "OBJCT" );
        break;
    case EntityKind::DRAW:
        return std::string ( "DRAW" );
        break;
    case EntityKind::EFFECT:
        return std::string ( "EFFECT" );
        break;
    case EntityKind::TEXTURE:
        return std::string ( "TEXTURE" );
        break;
    case EntityKind::PHYSICS:
        return std::string ( "PHYSICS" );
        break;
    case EntityKind::MATERIAL:
        return std::string ( "MATERIAL" );
        break;
    case EntityKind::CONSTRAINT:
        return std::string ( "CONSTRAINT" );
        break;
	case EntityKind::SCENEROOT:
		return std::string("SCENEROOT");
		break;
    }

    return std::string ( "Desconhecido" );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
