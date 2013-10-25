#include "Material.h"

namespace Chimera {

Material::Material ( void ) : Node ( EntityKind::MATERIAL ) {
}

Material::Material ( const Material &_cpy ) : Node ( _cpy ) {
}

Material::~Material() {
}

void Material::update ( DataMsg *dataMsg ) {
    Node::update ( dataMsg );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
