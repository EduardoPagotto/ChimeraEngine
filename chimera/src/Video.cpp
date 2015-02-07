#include "Video.h"

#include "ExceptionChimera.h"
//
// #include <SDL_opengl.h>
// #include <SDL_syswm.h>

#ifdef WIN32
#include "windows.h"
#endif

//#include "GL/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	Video::Video(std::string _nome) : nomeTela( _nome ) {

	}


// 	Video::Video( int _width, int _height, std::string _nome ) : nomeTela( _nome )  {
//
// 		rectangle.x = 0;
// 		rectangle.y = 0;
// 		rectangle.w = _width;
// 		rectangle.h = _height;
//
// 		initHardware();
//
// // 		rectangle.x = 0;
// // 		rectangle.y = 0;
// // 		rectangle.w = _width;
// // 		rectangle.h = _height;
// //
// // 		if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
// // 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o SDL:" + std::string( SDL_GetError() ) ) );
// //
// // 		// set the opengl context version
// // 		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
// // 		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
// //
// // 		// turn on double buffering set the depth buffer to 24 bits
// // 		// you may need to change this to 16 or 32 for your system
// // 		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
// // 		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
// //
// // 		window = SDL_CreateWindow( nomeTela.c_str(),    //    window title
// // 		                           SDL_WINDOWPOS_UNDEFINED,                //    initial x position
// // 		                           SDL_WINDOWPOS_UNDEFINED,                //    initial y position
// // 		                           rectangle.w,                                  //    width, in pixels
// // 		                           rectangle.h,                                 //    height, in pixels
// // 		                           SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ); //    flags - see below
// //
// // 		if ( window == nullptr )
// // 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao inicial a Janela:" + std::string( SDL_GetError() ) ) );
// //
// // 		context = SDL_GL_CreateContext( window );
// //
// // 		if ( context == nullptr )
// // 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao criar o contexto:" + std::string( SDL_GetError() ) ) );
// //
// // 		int interval = SDL_GL_SetSwapInterval( 1 );
// //
// // 		if ( interval < 0 )
// // 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Ajustar o VSync:" + std::string( SDL_GetError() ) ) );
//
// 	}

	Video::~Video() {

// 		if ( context != nullptr ) {
// 			SDL_GL_DeleteContext( context );
// 			context = nullptr;
// 		}
//
// 		if ( window != nullptr ) {
// 			SDL_DestroyWindow( window );
// 			window = nullptr;
// 		}
//
// 		SDL_Quit();
	}

// 	void Video::initGL() {
//
// 		GLenum error = GL_NO_ERROR;
//
// 		//Initialize Projection Matrix
// 		glMatrixMode( GL_PROJECTION );
// 		glLoadIdentity();
//
// 		//Check for error
// 		error = glGetError();
//
// 		if ( error != GL_NO_ERROR )
// 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o OpenGL:" + std::string( ( const char * ) gluErrorString( error ) ) ) );
//
// 		//Initialize Modelview Matrix
// 		glMatrixMode( GL_MODELVIEW );
// 		glLoadIdentity();
//
// 		//Check for error
// 		error = glGetError();
//
// 		if ( error != GL_NO_ERROR )
// 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o OpenGL:" + std::string( ( const char * ) gluErrorString( error ) ) ) );
//
// 		//Initialize clear color
// 		glClearColor( 0.f, 0.f, 0.f, 1.f );
//
// 		//Check for error
// 		error = glGetError();
//
// 		if ( error != GL_NO_ERROR )
// 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao Iniciar o OpenGL:" + std::string( ( const char * ) gluErrorString( error ) ) ) );
//
// 		//TODO retirar daqui e colocar na inicializacao do sceneMng
// 		//estado inicial do openGL
// 		glEnable( GL_TEXTURE_2D );
// 		glShadeModel( GL_SMOOTH );
// 		glClearDepth( 1.0f );
// 		glEnable( GL_DEPTH_TEST );
// 		glEnable( GL_CULL_FACE );
// 		glDepthFunc( GL_LEQUAL );
// 		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
// 		glEnable( GL_LIGHTING );
//
// 	}

// 	void Video::openFrameBuffer() {
//
// 		SDL_Rect renderTargetSize;
// 		renderTargetSize.h = rectangle.h;
// 		renderTargetSize.w = rectangle.w;
//
// 		glewExperimental = GL_TRUE;
//
// 		glewInit();
//
// 		//Cria o  Frame Buffer Object (FBO)
// 		glGenFramebuffers(1, &frameBufferObject);
// 		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject); //Aciona o FBO
//
// 		//Cria o Render Buffer Object (RBO)
// 		glGenRenderbuffers(1, &renderBuffer);
// 		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);//Aciona o RBO
// 		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);//aloca espaco no render buffer
//
// 		//Cria a textura no tamanho e cores definidos na da tela
// 		glGenTextures(1, &texture);
// 		glBindTexture(GL_TEXTURE_2D, texture);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//
// 		//anexa a textura para o FBO (color attachement point)
// 		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
// 		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
//
// 		//anexa o RBO (depth attachement point)
// 		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
//
// 		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
// 		glDrawBuffers(1, DrawBuffers);
//
// 		//Verifica o status do FBO
// 		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 		{
// 			closeFrameBuffer();
//
// 			//SDL_GL_DeleteContext(context);
// 			//SDL_DestroyWindow(window);
//
// 			//ovrHmd_Destroy(hmd);
// 			//ovr_Shutdown();
//
// 			//SDL_Quit();
// 			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao instanciar o framebuffer"));
//
// 		}
// 	}

// 	void Video::closeFrameBuffer() {
//
// 		glDeleteFramebuffers(1, &frameBufferObject);
// 		glDeleteTextures(1, &texture);
// 		glDeleteRenderbuffers(1, &renderBuffer);
//
// 	}

// 	void Video::initDraw(){
//
//  		//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
// 		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//
// 	}
//
// 	void Video::endDraw()
// 	{
// 		SDL_GL_SwapWindow( window );
// 	}

// 	void Video::setViewPortOrtogonal() {
// 		glViewport( rectangle.x, rectangle.y, rectangle.w, rectangle.h );
// 		glMatrixMode( GL_PROJECTION );
// 		glLoadIdentity();
// 		glOrtho( rectangle.x, rectangle.w, rectangle.y, rectangle.h, -1.0, 1.0 );
// 		glMatrixMode( GL_MODELVIEW );
// 		glLoadIdentity();
// 	}

	void Video::setViewPortOrtogonal( const int &_x, const int &_y, const int &_w, const int &_h ) {
		glViewport( _x, _y, _w, _h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( _x, _w, _y, _h, -1.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	void Video::setViewPortPerspective( const int &_x, const int &_y, const int &_w, const int &_h, const float &_fov, const float &_near, const float &_far ) {
		glViewport( _x, _y, _w, _h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( _fov, ( GLfloat )( float ) _w / ( float ) _h, _near, _far );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	std::string Video::getVersaoOpenGL() {

		std::string retorno = "";
		const char *version = ( const char * ) glGetString( GL_VERSION );

		if ( version != nullptr )
			retorno.append( version );
		else {
			//Check for error
			GLenum error = glGetError();
			throw ExceptionChimera( ExceptionCode::READ, std::string( ( const char * ) gluErrorString( error )));
		}

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

// 	void Video::initScene()  {
// 		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
// 	}

} /* namespace Chimera */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4;


