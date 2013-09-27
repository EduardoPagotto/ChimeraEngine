#include "Image.h"

namespace Chimera {

Image::Image() {
    pImage = NULL;
}

Image::~Image() {
    if ( pImage ) {
        SDL_FreeSurface ( pImage );
        pImage = NULL;
    }
}

int Image::load ( const char *_file ) {
    pImage = IMG_Load ( _file );
    if ( pImage!=NULL )
        return 0;

    return -1;
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
