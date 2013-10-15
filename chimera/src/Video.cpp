#include "Video.h"

namespace Chimera {

Video::Video ( int _width, int _height, std::string _nome ) :
    width ( _width ), height ( _height ), nomeTela ( _nome ),fps(0)  {

    window = SDL_CreateWindow ( nomeTela.c_str(),   //    window title
                                SDL_WINDOWPOS_UNDEFINED,                //    initial x position
                                SDL_WINDOWPOS_UNDEFINED,                //    initial y position
                                width,                                  //    width, in pixels
                                height,                                 //    height, in pixels
                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ); //    flags - see below
    
    if ( window != nullptr ) {
        
        context = SDL_GL_CreateContext ( window );
        if ( context == nullptr ) {
            throw ExceptionSDL ( ExceptionCode::CREATE, std::string ( SDL_GetError() ) );
        }
                
    } else {
        throw ExceptionSDL ( ExceptionCode::CREATE, std::string ( SDL_GetError() ) );
    }
    
    timerFPS.setElapsedCount ( 1000 );
    timerFPS.start();
    
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

int Video::getFPS () {

    if ( timerFPS.stepCount() == true ) {
        fps = timerFPS.getCountStep();
    }
    
    return fps;
}

void Video::swapWindow() {
    SDL_GL_SwapWindow ( window );
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
