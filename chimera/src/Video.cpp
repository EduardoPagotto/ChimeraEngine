#include "Video.h"

#include "ExceptionSDL.h"

#include <SDL_opengl.h>
#include <SDL_syswm.h>

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

	Video::~Video() {
		
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

	void Video::openFrameBuffer() {
		
		SDL_Rect renderTargetSize;
		renderTargetSize.h = rectangle.h;
		renderTargetSize.w = rectangle.w;
		
		glewExperimental = GL_TRUE;
		
		glewInit();
		
		//GLuint frameBuffer;
		glGenFramebuffers(1, &frameBuffer);
		
		//GLuint texture;
		glGenTextures(1, &texture);
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		
		//GLuint renderBuffer;
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			closeFrameBuffer();
			
			//SDL_GL_DeleteContext(context);
			//SDL_DestroyWindow(window);
			
			//ovrHmd_Destroy(hmd);
			//ovr_Shutdown();
			
			//SDL_Quit();
			throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao instanciar o framebuffer"));
			
		}	
	}
	
	void Video::closeFrameBuffer() {
		
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &texture);
		glDeleteRenderbuffers(1, &renderBuffer);
		
	}
	
	void Video::initDraw(){
		
 		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
	}
	
	void Video::endDraw()
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

// 	void Video::initScene()  {
// 		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
// 	}
	
} /* namespace Chimera */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4;

/*
 * VERIFICAR!!!!!!
// create frame buffer object
glGenFramebuffers(1, frameBufferObject);

// create empty texture
int width = 512;
int height = 512;
int numberOfChannels = 3;
GLuint internalFormat = GL_RGB8;
GLuint format = GL_RGB;

unsigned char* texels = new unsigned char[width * height * numberOfChannels];

glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, texels);

delete[] texels;
texels = NULL;

// draw the colored quad into the initially empty texture
glDisable(GL_CULL_FACE);
glDisable(GL_DEPTH_TEST);

// store attibutes
glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// reset viewport
glViewport(0, 0, width, height);

// setup modelview matrix
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();

// setup projection matrix
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();

// setup orthogonal projection
glOrtho(-width / 2, width / 2, -height / 2, height / 2, 0, 1000);

// bind framebuffer object
glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

// attach empty texture to framebuffer object
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

// check framebuffer status (see above)

// bind framebuffer object (IMPORTANT! bind before adding color attachments!)
glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

// define render targets (empty texture is at GL_COLOR_ATTACHMENT0)
glDrawBuffers(1, GL_COLOR_ATTACHMENT0); // you can of course have more than only 1 attachment

// activate & bind empty texture
// I figured activating and binding must take place AFTER attaching texture as color attachment
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);

// clear color attachments
glClear(GL_COLOR_BUFFER_BIT);

// make background yellow
glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

// draw quad into texture attached to frame buffer object
glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
glBegin(GL_QUADS);
glColor4f(1.0f, 1.0f, 1.0f, 1.0f); glVertex2f(0.0f, 100.0f); // top left
glColor4f(1.0f, 0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.0f); // bottom left
glColor4f(0.0f, 1.0f, 0.0f, 1.0f); glVertex2f(100.0f, 0.0f); // bottom right
glColor4f(0.0f, 0.0f, 1.0f, 1.0f); glVertex2f(100.0f, 100.0f); // top right
glEnd();

// reset projection matrix
glMatrixMode(GL_PROJECTION);
glPopMatrix();

// reset modelview matrix
glMatrixMode(GL_MODELVIEW);
glPopMatrix();

// restore attributes
glPopAttrib();

glEnable(GL_DEPTH_TEST);
glEnable(GL_CULL_FACE);

// I guess, now it's OK to create MipMaps

// draw the scene
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();

glColor4f(1.0, 1.0, 1.0, 1.0);

// begin texture mapping
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

// normal faces "camera"
glNormal3d(0.0f, 0.0f, 1.0f);

glBegin(GL_QUADS);
glNormal3d(0.0f, 0.0f, 1.0f);
glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 50.0f, -100.0f);    // top left
glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -100.0f);     // bottom left
glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 0.0f, -100.0f);    // bottom right
glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 50.0f, -100.0f);   // top right
glEnd();

glDisable(GL_TEXTURE_2D);

glPopMatrix();

// finish rendering
glFlush();
glFinish();

// swap buffers (I forgot to mention that I use SDL)
SDL_GL_SwapBuffers();
*/
// do the clean up!
