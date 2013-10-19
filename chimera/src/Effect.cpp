#include "Effect.h"

namespace Chimera {

Effect::Effect() {
    
    shininess = 10.5f;
    diffuse = Color::BLACK;
    ambient = Color::BLACK;
    specular = Color::BLACK;
    emissive = Color::BLACK;
    nameTextureId = "";      
    
}

Effect::Effect(const Effect& _cpy) {
    
    diffuse = _cpy.diffuse;
    ambient = _cpy.ambient;
    specular = _cpy.specular;
    emissive = _cpy.emissive;
    shininess = _cpy.shininess; 
    nameTextureId = _cpy.nameTextureId;   
}

void Effect::render() {
    
    if ( glIsEnabled ( GL_COLOR_MATERIAL ) == 1 ) {
        glMaterialfv ( GL_FRONT, GL_AMBIENT, ambient.ptr() );
        glMaterialfv ( GL_FRONT, GL_DIFFUSE, diffuse.ptr() );
        glMaterialfv ( GL_FRONT, GL_SPECULAR, specular.ptr() );
        glMaterialfv ( GL_FRONT, GL_SHININESS,  &shininess );
        glMaterialfv ( GL_FRONT, GL_EMISSION, emissive.ptr() ); 
    }
    
}

// void Effect::update ( DataMsg *dataMsg ) {
//     
//     if (dataMsg->getKindOp()==KindOp::START) {
// 
//     } else if (dataMsg->getKindOp()==KindOp::DRAW3D) {
//         
//         if ( glIsEnabled ( GL_COLOR_MATERIAL ) == 1 ) {
//             glMaterialfv ( GL_FRONT, GL_AMBIENT, ambient.ptr() );
//             glMaterialfv ( GL_FRONT, GL_DIFFUSE, diffuse.ptr() );
//             glMaterialfv ( GL_FRONT, GL_SPECULAR, specular.ptr() );
//             glMaterialfv ( GL_FRONT, GL_SHININESS,  &shininess );
//             glMaterialfv ( GL_FRONT, GL_EMISSION, emissive.ptr() ); 
//        }
//             
//     }
//     
//     Node::update(dataMsg);
//     
// }

}