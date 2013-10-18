#include "Image.h"

namespace Chimera {

Image::Image() : Node ( EntityKind::IMAGE ) , pImage(nullptr),idTexturaGL(0) {
}

Image::Image ( const Image &_imagen ) : Node ( _imagen ), pImage(_imagen.pImage),idTexturaGL(0) {
}

Image::~Image() {
    if ( pImage != nullptr ) {
        SDL_FreeSurface ( pImage );
        pImage = nullptr;
    }
}

int Image::link () {
    
    //Necessario ser chamado antes do desenho??
    if ( idTexturaGL >0 )
        glBindTexture ( GL_TEXTURE_2D, idTexturaGL );

    return idTexturaGL;
    
}

void Image::update ( DataMsg *dataMsg ) {

    Node::update ( dataMsg );

    if ( dataMsg->getKindOp() ==KindOp::START ) {
        
        //antigo -> initTex();
        glGenTextures ( 1, &idTexturaGL );
        //  //glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
        glBindTexture ( GL_TEXTURE_2D, idTexturaGL );
        //glTexImage2D(GL_TEXTURE_2D, 0, 3,image->w, image->h, 0, GL_BGR,GL_UNSIGNED_BYTE, image->pixels);
        gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, pImage->w,
                            pImage->h,
                            GL_RGB,
                            GL_UNSIGNED_BYTE,
                            pImage->pixels );
        
        //TODO:carregar conf do arquivo collada
        glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
        
    } else if (dataMsg->getKindOp()==KindOp::DRAW3D) {
        
        link();
        
    }
}

void Image::loadImage () {

    pImage = IMG_Load ( pathFile.c_str() );
    if ( pImage != nullptr )
        return;

    throw ExceptionChimera ( ExceptionCode::READ, "Falha ao ler arquivo:"+pathFile );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
