#include "Material.h"

namespace Chimera {

Material::Material ( void ) : Node ( EntityKind::MATERIAL ) {
}

Material::Material ( const Material &_cpy ) : Node(_cpy) {
}

Material::~Material() {
}

void Material::update ( DataMsg *dataMsg ) {
    Node::update(dataMsg);
}

// void Material::exec ( void ) {
//     link();//FIXME esta agora em imagem!!!!
// 
// //     float val = effect.getShininess();
// // 
// //     glMaterialfv ( GL_FRONT, GL_AMBIENT, effect.getAmbient().ptr() );
// //     glMaterialfv ( GL_FRONT, GL_DIFFUSE, effect.getDiffuse().ptr() );
// //     glMaterialfv ( GL_FRONT, GL_SPECULAR, effect.getSpecular().ptr() );
// //     glMaterialfv ( GL_FRONT, GL_SHININESS, &val );
// // 
// //     glMaterialfv ( GL_FRONT, GL_EMISSION, effect.getEmissive().ptr() );
// }

// int Material::link ( void ) {
// 
// //     if ( idTex >0 )
// //         glBindTexture ( GL_TEXTURE_2D,idTex );
// // 
// //     return idTex;
// 
// }

// void Material::initTex ( void ) {
//     
//     if ( pTexture ) {
//         glGenTextures ( 1, &idTex );
//         //   //glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
//         glBindTexture ( GL_TEXTURE_2D,idTex );
//         //glTexImage2D(GL_TEXTURE_2D, 0, 3,image->w, image->h, 0, GL_BGR,GL_UNSIGNED_BYTE, image->pixels);
//         gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, pTexture->pImage->w,
//                             pTexture->pImage->h,
//                             GL_RGB,
//                             GL_UNSIGNED_BYTE,
//                             pTexture->pImage->pixels );
// 
//         //TODO:carregar conf do arquivo collada
//         glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
//         glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
//     }
// 
// }

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
