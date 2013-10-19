#include "Video.h"

namespace Chimera {

Video::Video ( int _width, int _height, std::string _nome ) : nomeTela ( _nome )  {

    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = _width;
    rectangle.h = _height;


    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == 0 ) {

        // set the opengl context version
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

        // turn on double buffering set the depth buffer to 24 bits
        // you may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
        SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );

        window = SDL_CreateWindow ( nomeTela.c_str(),   //    window title
                                    SDL_WINDOWPOS_UNDEFINED,                //    initial x position
                                    SDL_WINDOWPOS_UNDEFINED,                //    initial y position
                                    rectangle.w,                                  //    width, in pixels
                                    rectangle.h,                                 //    height, in pixels
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ); //    flags - see below

        if ( window != nullptr ) {

            context = SDL_GL_CreateContext ( window );
            if ( context != nullptr ) {


                return;

            }
        }
    }

    throw ExceptionSDL ( ExceptionCode::CREATE, std::string ( SDL_GetError() ) );
}

Video::~Video() {

    if ( context != nullptr ) {
        SDL_GL_DeleteContext ( context );
        context = nullptr;
    }

    if ( window != nullptr ) {
        SDL_DestroyWindow ( window );
        window = nullptr;
    }

}

void Video::swapWindow() {
    SDL_GL_SwapWindow ( window );
}

} /* namespace Chimera */

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
