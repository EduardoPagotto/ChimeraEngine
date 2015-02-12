#include "VideoDevice.h"

#include "ExceptionSDL.h"

#ifdef WIN32
#include "windows.h"
#endif

//#include "GL/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	VideoDevice::VideoDevice(std::string _nome) : Video( _nome) {

		rectangle.x = 0;
		rectangle.y = 0;
		rectangle.w = SDL_WINDOWPOS_CENTERED;
		rectangle.h = SDL_WINDOWPOS_CENTERED;

		kindDevice = KIND_DEVICE::SCREEN;

		initHardware();

	}

	VideoDevice::VideoDevice( int _width, int _height, std::string _nome ) : Video( _nome) {

		rectangle.x = 0;
		rectangle.y = 0;
		rectangle.w = _width;
		rectangle.h = _height;

		kindDevice = KIND_DEVICE::SCREEN;

		initHardware();

	}

	VideoDevice::~VideoDevice() {

		if ( context != nullptr ) {
			SDL_GL_DeleteContext( context );
			context = nullptr;
		}

		if ( window != nullptr ) {
			SDL_DestroyWindow( window );
			window = nullptr;
		}

		SDL_Quit();

	}

	void VideoDevice::initHardware() {

		if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o SDL:" + std::string( SDL_GetError() ) ) );

		// set the opengl context version
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		// turn on double buffering set the depth buffer to 24 bits
		// you may need to change this to 16 or 32 for your system
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

		window = SDL_CreateWindow( nomeTela.c_str(),    //    window title
								   SDL_WINDOWPOS_UNDEFINED,                //    initial x position
							 SDL_WINDOWPOS_UNDEFINED,                //    initial y position
							 rectangle.w,                                  //    width, in pixels
							 rectangle.h,                                 //    height, in pixels
							 SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ); //    flags - see below

		if ( window == nullptr )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao inicial a Janela:" + std::string( SDL_GetError() ) ) );

		context = SDL_GL_CreateContext( window );

		if ( context == nullptr )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao criar o contexto:" + std::string( SDL_GetError() ) ) );

		int interval = SDL_GL_SetSwapInterval( 1 );

		if ( interval < 0 )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Ajustar o VSync:" + std::string( SDL_GetError() ) ) );

	}

	void VideoDevice::initGL() {

		GLenum error = GL_NO_ERROR;

		//Initialize Projection Matrix
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		//Check for error
		error = glGetError();

		if ( error != GL_NO_ERROR )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o OpenGL:" + std::string( ( const char * ) gluErrorString( error ) ) ) );

		//Initialize Modelview Matrix
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		//Check for error
		error = glGetError();

		if ( error != GL_NO_ERROR )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o OpenGL:" + std::string( ( const char * ) gluErrorString( error ) ) ) );

		//Initialize clear color
		glClearColor( 0.f, 0.f, 0.f, 1.f );

		//Check for error
		error = glGetError();

		if ( error != GL_NO_ERROR )
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o OpenGL:" + std::string( ( const char * ) gluErrorString( error ) ) ) );

		//TODO retirar daqui e colocar na inicializacao do sceneMng
		//estado inicial do openGL
		glEnable( GL_TEXTURE_2D );
		glShadeModel( GL_SMOOTH );
		glClearDepth( 1.0f );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );
		glDepthFunc( GL_LEQUAL );
		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
		glEnable( GL_LIGHTING );

	}

	void VideoDevice::initDraw(){

		//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	}

	void VideoDevice::endDraw()
	{
		SDL_GL_SwapWindow( window );
	}

	void VideoDevice::getGeometry(int &_x, int &_y, int &_w, int &_h, int index) {

		_x = rectangle.x;
		_y = rectangle.y;
		_w = rectangle.w;
		_h = rectangle.h;

	}

	void VideoDevice::executeViewPerspective( Camera *pCamera, int _eye ) {

		glViewport( rectangle.x, rectangle.y, rectangle.w, rectangle.h );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( pCamera->getFov(), ( GLfloat )( float ) rectangle.w / ( float ) rectangle.h, pCamera->getNear(), pCamera->getFar() );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

	}


}