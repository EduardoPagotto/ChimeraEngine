/*
 * Video.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: locutus
 */

#include "Video.h"

namespace Chimera {

Video::Video ( int width, int height, std::string nome ) :
    width ( width ), height ( height ), nomeTela ( nome )  {

    window = nullptr;
    context = nullptr;

}

Video::~Video() {

}

void Video::initGL() {
    glShadeModel ( GL_SMOOTH );
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth ( 1.0f );
    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_CULL_FACE );
    glDepthFunc ( GL_LEQUAL );
    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void Video::renderize ( void ) {

}

void Video::open() {

    // set the opengl context version
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

    // turn on double buffering set the depth buffer to 24 bits
    // you may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );


    window = SDL_CreateWindow ( nomeTela.c_str(), 	//    window title
                                SDL_WINDOWPOS_UNDEFINED,           		//    initial x position
                                SDL_WINDOWPOS_UNDEFINED,           		//    initial y position
                                width,                           		//    width, in pixels
                                height,                             	//    height, in pixels
                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );	//    flags - see below

    if ( window != nullptr ) {

        context = SDL_GL_CreateContext ( window );
        if ( context!=nullptr ) {

            const unsigned char* version = glGetString ( GL_VERSION );
            if ( version != NULL ) {
                std::cout << "Versao OpenGL: " << version << std::endl;
            } else {
                std::cout << "Falha ao determinar Versao OpenGL"  << std::endl;
            }

        } else {
            throw ExceptionSDL ( -3, std::string ( SDL_GetError() ) );
        }


    } else {
        throw ExceptionSDL ( -2, std::string ( SDL_GetError() ) );
    }

    // sync buffer swap with monitor's vertical refresh rate
    SDL_GL_SetSwapInterval ( 1 );

    initGL();

    timerFPS.setElapsedCount ( 1000 );
    timerFPS.start();

}

void Video::execute () {

    if ( timerFPS.stepCount() == true ) {
        std::cout << "FPS: " << timerFPS.getCountStep() << std::endl;
    }
}

void Video::swapWindow() {
    SDL_GL_SwapWindow ( window );
}

void Video::close() {

    if ( context != nullptr ) {
        SDL_GL_DeleteContext ( context );
        context = nullptr;
    }

    if ( window != nullptr ) {
        SDL_DestroyWindow ( window );
        window = nullptr;
    }

    //SDL_Quit();
}

} /* namespace Chimera */

//	if (render != nullptr) {
//		SDL_DestroyRenderer(render);
//		render = nullptr;
//	}
//void Video::initGL() {
//	/* Enable smooth shading */
//	glShadeModel(GL_SMOOTH);
//
//	/* Set the background black */
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//	/* Depth buffer setup */
//	glClearDepth(1.0f);
//
//	/* Enables Depth Testing */
//	glEnable(GL_DEPTH_TEST);
//
//	/* The Type Of Depth Test To Do */
//	glDepthFunc(GL_LEQUAL);
//
//	/* Really Nice Perspective Calculations */
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//}

/*
int Video::setViewport() {

	GLfloat ratio;


	if (height == 0) {
		height = 1;
	}

	ratio = (GLfloat) width / (GLfloat) height;


	glViewport(0, 0, (GLsizei) width, (GLsizei) height);


	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();


	//gluPerspective(45.0f, ratio, 0.1f, 100.0f);


	glMatrixMode( GL_MODELVIEW);


	glLoadIdentity();

	return 1;
}
*/

//void Video::renderize() {
//
//	/* Set the background black */
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	/* Clear The Screen And The Depth Buffer */
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	/* Move Left 1.5 Units And Into The Screen 6.0 */
//	glLoadIdentity();
//	glTranslatef(-1.5f, 0.0f, -6.0f);
//
//	glBegin( GL_TRIANGLES); /* Drawing Using Triangles */
//	glVertex3f(0.0f, 1.0f, 0.0f); /* Top */
//	glVertex3f(-1.0f, -1.0f, 0.0f); /* Bottom Left */
//	glVertex3f(1.0f, -1.0f, 0.0f); /* Bottom Right */
//	glEnd(); /* Finished Drawing The Triangle */
//
//	/* Move Right 3 Units */
//	glTranslatef(3.0f, 0.0f, 0.0f);
//
//	glBegin( GL_QUADS); /* Draw A Quad */
//	glVertex3f(-1.0f, 1.0f, 0.0f); /* Top Left */
//	glVertex3f(1.0f, 1.0f, 0.0f); /* Top Right */
//	glVertex3f(1.0f, -1.0f, 0.0f); /* Bottom Right */
//	glVertex3f(-1.0f, -1.0f, 0.0f); /* Bottom Left */
//	glEnd(); /* Done Drawing The Quad */
//	SDL_RenderPresent(render);
//
//}

//		render = SDL_CreateRenderer(window, -1,
//				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//
//		if (render != nullptr) {
//
//			SDL_RendererInfo displayRendererInfo;
//			SDL_GetRendererInfo(render, &displayRendererInfo);
//
//			/*TODO: Check that we have OpenGL */
//			if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0
//					|| (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE)
//							== 0) {
//				/*TODO: Handle this. We have no render surface and not accelerated. */
//				std::cout << "Sem Open GL " << std::endl;
//			}
//
//			//SDL_RenderClear(render);
//
//			//initGL();
//
//		} else {
//
//			throw ExceptionSDL(-3, std::string(SDL_GetError()));
//		}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
