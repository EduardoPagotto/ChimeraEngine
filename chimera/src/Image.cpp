#include "Image.h"

namespace Chimera {

Image::Image() : Node ( EntityKind::IMAGE ) {

    pImage = nullptr;

}

Image::Image ( const Image &_imagen ) : Node ( _imagen ) {

    pImage = _imagen.pImage;

}

Image::~Image() {

    if ( pImage != nullptr ) {

        SDL_FreeSurface ( pImage );
        pImage = nullptr;

    }

}

void Image::update ( DataMsg *dataMsg ) {

    Node::update ( dataMsg );

    if ( dataMsg->getKindOp() ==KindOp::START ) {

        //         if ( pMaterial )
        //             pMaterial->initTex();

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
