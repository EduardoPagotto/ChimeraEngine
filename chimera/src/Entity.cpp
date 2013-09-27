#include "Entity.h"

namespace Chimera {

unsigned Entity::serialMaster = 0;
std::list<Entity*> Entity::lObj;

Entity *Entity::findObj ( EntityType type,std::string name ) {
    for ( Entity *entity : lObj ) {
        std::string nome = entity->getName();
        if ( ( entity->getType() ==type ) && ( nome.compare ( name ) ==true ) )
            return entity;
    }
    return nullptr;
}

Entity *Entity::findObj ( std::string name ) {

    for ( Entity *entity : lObj ) {
        std::string nome = entity->getName();
        if ( nome.compare ( name ) ==true )
            return entity;
    }
    return nullptr;
}


}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
