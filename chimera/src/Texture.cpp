#include "Texture.h"

namespace Chimera {

Texture::Texture() :  pImage(nullptr),idTexturaGL(0) {
    pathFile = "none";
}

Texture::Texture ( const Texture &_texture ) : pImage(_texture.pImage), idTexturaGL(_texture.idTexturaGL), pathFile(_texture.pathFile) {
}

Texture::~Texture() {
    if ( pImage != nullptr ) {
        SDL_FreeSurface ( pImage );
        pImage = nullptr;
    }
}

int Texture::render() {
    
    //Necessario ser chamado antes do desenho??
//     if ( idTexturaGL >0 )
//         glBindTexture ( GL_TEXTURE_2D, idTexturaGL );

    glBindTexture( GL_TEXTURE_2D, texture[2] );
    return idTexturaGL;
    
}

int Texture::init() {
    
    loadImage();
    
    /* Create The Texture */
    glGenTextures( 3, &texture[0] );
    
    /* Load in texture 1 */
    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    
    /* Generate The Texture */
    glTexImage2D( GL_TEXTURE_2D, 0, 3,  pImage->w,
                  pImage->h, 0, GL_BGR,
                  GL_UNSIGNED_BYTE,  pImage->pixels );
    
    /* Nearest Filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                     GL_NEAREST );
    
    /* Load in texture 2 */
    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture( GL_TEXTURE_2D, texture[1] );
    
    /* Linear Filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                     GL_LINEAR );
    
    /* Generate The Texture */
    glTexImage2D( GL_TEXTURE_2D, 0, 3,  pImage->w,
                  pImage->h, 0, GL_BGR,
                  GL_UNSIGNED_BYTE,  pImage->pixels );
    
    /* Load in texture 3 */
    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture( GL_TEXTURE_2D, texture[2] );
    
    /* Mipmapped Filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                     GL_LINEAR );
    
    /* Generate The MipMapped Texture ( NEW ) */
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3,  pImage->w,
                       pImage->h, GL_BGR,
                       GL_UNSIGNED_BYTE,  pImage->pixels );
    
    SDL_FreeSurface( pImage );
//     //antigo -> initTex();
//     glGenTextures ( 1, &idTexturaGL );
//     //  //glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
//     glBindTexture ( GL_TEXTURE_2D, idTexturaGL );
//     //glTexTexture2D(GL_TEXTURE_2D, 0, 3,image->w, image->h, 0, GL_BGR,GL_UNSIGNED_BYTE, image->pixels);
//     gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, pImage->w,
//                         pImage->h,
//                         GL_RGB,
//                         GL_UNSIGNED_BYTE,
//                         pImage->pixels );
//     
//     //TODO:carregar conf do arquivo collada
//     glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
//     glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    
}

void Texture::loadImage () {

    pImage = IMG_Load ( pathFile.c_str() );
    if ( pImage != nullptr )
        return;

    throw ExceptionChimera ( ExceptionCode::READ, "Falha ao ler arquivo:"+pathFile );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
