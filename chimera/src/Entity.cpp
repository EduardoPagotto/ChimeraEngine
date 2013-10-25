#include "Entity.h"

namespace Chimera {

unsigned Entity::serialMaster = 0;

std::string Entity::getNameKindNode(EntityKind _kind) {
    
    switch (_kind) {
        case EntityKind::NODE:
            return std::string("NODE");
            break;
        case EntityKind::CAMERA:
            return std::string("CAMERA");
            break;
        case EntityKind::LIGHT:
            return std::string("LIGHT");
            break;
        case EntityKind::OBJECT:
            return std::string("OBJCT");
            break;
    }
    
    return std::string("Desconhecido");
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
