#include "Video.h"

#include "ExceptionSDL.h"

#include <SDL_opengl.h>

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	Video::Video( int _width, int _height, std::string _nome ) : nomeTela( _nome )  {

		rectangle.x = 0;
		rectangle.y = 0;
		rectangle.w = _width;
		rectangle.h = _height;

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

	void Video::initGL() {

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

	Video::~Video() {

		if ( context != nullptr ) {
			SDL_GL_DeleteContext( context );
			context = nullptr;
		}

		if ( window != nullptr ) {
			SDL_DestroyWindow( window );
			window = nullptr;
		}

	}

	void Video::swapWindow() {
		//TODO remover e colocar no sceneMng
		SDL_GL_SwapWindow( window );
	}

	void Video::setViewPortOrtogonal() {
		glViewport( rectangle.x, rectangle.y, rectangle.w, rectangle.h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( rectangle.x, rectangle.w, rectangle.y, rectangle.h, -1.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	void Video::setViewPortOrtogonal( const SDL_Rect &_rectangle ) {
		glViewport( _rectangle.x, _rectangle.y, _rectangle.w, _rectangle.h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( _rectangle.x, _rectangle.w, _rectangle.y, _rectangle.h, -1.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	void Video::setViewPortPerspective( const float &_fov, const float &_near, const float &_far ) {
		glViewport( rectangle.x, rectangle.y, rectangle.w, rectangle.h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( _fov, ( GLfloat )( float ) rectangle.w / ( float ) rectangle.h, _near, _far );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	void Video::setViewPortPerspective( const SDL_Rect &_rectangle, const float &_fov, const float &_near, const float &_far ) {
		glViewport( _rectangle.x, _rectangle.y, _rectangle.w, _rectangle.h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( _fov, ( GLfloat )( float ) _rectangle.w / ( float ) _rectangle.h, _near, _far );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	std::string Video::getVersaoOpenGL() {

		std::string retorno = "";
		const char *version = ( const char * ) glGetString( GL_VERSION );

		if ( version != nullptr )
			retorno.append( version );
		else
			throw ExceptionSDL( ExceptionCode::READ, std::string( SDL_GetError() ) );

		return retorno;
	}

	void Video::begin2D() {
		//Salva flags openGL
		glPushAttrib( GL_ENABLE_BIT );

		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );

		glDisable( GL_COLOR_MATERIAL );
		glDisable( GL_LIGHTING );

		//TODO problemas de contaminacao de cor no HUD
		//glEnable(GL_TEXTURE_2D);
		glDisable( GL_TEXTURE_2D );

		// This allows alpha blending of 2D textures with the scene
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}

	void Video::end2D() {
		//Retorna para estado de maquina 3d
		glPopAttrib();

		//TODO Corrigir reaster problemas de constaminacao de cor no 3D
		glColor3f( 1.0, 1.0, 1.0 );
	}

	void Video::setLight( bool _lightOn ) {
		if ( _lightOn == true )
			glEnable( GL_LIGHTING );
		else
			glDisable( GL_LIGHTING );
	}

	void Video::setMaterial( bool _materialOn ) {

		if ( _materialOn == true ) {
			glDisable( GL_COLOR_MATERIAL );
			//glColorMaterial ( GL_FRONT, GL_DIFFUSE ); //TODO verificar necessidade
		} else {
			glEnable( GL_COLOR_MATERIAL );
			glColorMaterial( GL_FRONT, GL_DIFFUSE );  //TODO verificar necessidade
		}
	}

	void Video::initScene()  {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

} /* namespace Chimera */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4;
