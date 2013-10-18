#include "Effect.h"

namespace Chimera {

Effect::Effect() : Node(EntityKind::EFFECT) {
    
    shininess = 10.5f;
    diffuse = Color::BLACK;
    ambient = Color::BLACK;
    specular = Color::BLACK;
    emissive = Color::BLACK;
    nameTextureId = "";      
    
}

Effect::Effect(const Effect& _cpy) : Node(_cpy) {
    
    diffuse = _cpy.diffuse;
    ambient = _cpy.ambient;
    specular = _cpy.specular;
    emissive = _cpy.emissive;
    shininess = _cpy.shininess; 
    nameTextureId = _cpy.nameTextureId;   
}

void Effect::update ( DataMsg *dataMsg ) {
    
    Node::update(dataMsg);
    
    if (dataMsg->getKindOp()==KindOp::START) {
//         if ( pMaterial )
//             pMaterial->initTex();
    }
    
}

}