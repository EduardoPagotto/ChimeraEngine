#include "Game.h"
#include "ExceptionSDL.h"
#include "Shader.h"

#include "Game.h"
#include "ExceptionSDL.h"

#include "Transform.h"
#include "OpenGLDefs.h"

Game::Game(Chimera::SceneMng *_pScenMng) : GameClient(_pScenMng) {
}

Game::~Game() {

	    // Cleanup VBO
	    glDeleteBuffers ( 1, &vertexbuffer );
	    glDeleteVertexArrays ( 1, &VertexArrayID );
}

void Game::joystickCapture(Chimera::JoystickManager &joy) {}

void Game::joystickStatus(Chimera::JoystickManager &joy) {}

void Game::keyCapture(SDL_Keycode tecla) {

	switch (tecla) {
	case SDLK_ESCAPE:
		SDL_Event l_eventQuit;
		l_eventQuit.type = SDL_QUIT;
		if (SDL_PushEvent(&l_eventQuit) == -1) {
			throw Chimera::ExceptionSDL(Chimera::ExceptionCode::CLOSE, std::string(SDL_GetError()));
		}
		break;
	case SDLK_F1:
		//pHUD->setOn(!pHUD->isOn());
		break;
	case SDLK_F10:
		sendMessage(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
		break;
	case SDLK_UP:
		break;
	case SDLK_DOWN:
		break;
	case SDLK_LEFT:
		break;
	case SDLK_RIGHT:
		break;
	case SDLK_a:
		break;
	case SDLK_s:
		break;
	default:
		break;
	}
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {}

void Game::start() {

	//GameClient::start();

	//---------

	// Dark blue background
	glClearColor ( 0.0f, 0.0f, 0.4f, 0.0f );
	
	glGenVertexArrays ( 1, &VertexArrayID );
	glBindVertexArray ( VertexArrayID );
	
	// Create and compile our GLSL program from the shaders
#ifdef WIN32
	programID = Chimera::LoadShaders ( "C:\\Projetos\\ChimeraEngine\\AppShader\\shader\\SimpleVertexShader.vertexshader",
	                                    "C:\\Projetos\\ChimeraEngine\\AppShader\\shader\\SimpleFragmentShader.fragmentshader" );
# else
	programID = Chimera::LoadShaders ( "/home/locutus/Projetos/ChimeraEngine/AppShader/shader/SimpleVertexShader.vertexshader",
	                                    "/home/locutus/Projetos/ChimeraEngine/AppShader/shader/SimpleFragmentShader.fragmentshader" );
# endif
	
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
	    -1.0f, -1.0f, 0.0f,
	    1.0f, -1.0f, 0.0f,
	    0.0f, 1.0f, 0.0f,
	};
	
	// This will identify our vertex buffer
	//GLuint vertexbuffer;
	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers ( 1, &vertexbuffer );
	
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer ( GL_ARRAY_BUFFER, vertexbuffer );
	
	// Give our vertices to OpenGL.
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );


	//---------

	//Chimera::Transform *pOrigem = new Chimera::Transform(nullptr, "origem");
	//pSceneMng->cameraAtiva(nullptr);
	//pSceneMng->objetoAtivo(pOrigem);
}

void Game::stop() {
	GameClient::stop();
}

void Game::newFPS(const unsigned int &fps) {

	GameClient::newFPS(fps);
}

void Game::render() {

	// Clear the screen
	glClear ( GL_COLOR_BUFFER_BIT );
	
	// Use our shader
	glUseProgram ( programID );
	
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray ( 0 );
	glBindBuffer ( GL_ARRAY_BUFFER, vertexbuffer );
	glVertexAttribPointer (
	    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	    3,                  // size
	    GL_FLOAT,           // type
	    GL_FALSE,           // normalized?
	    0,                  // stride
	    ( void* ) 0         // array buffer offset
	);
	
	// Draw the triangle !
	glDrawArrays ( GL_TRIANGLES, 0, 3 ); // Starting from vertex 0; 3 vertices total -> 1 triangle
	
	glDisableVertexAttribArray ( 0 );

	//pSceneMng->draw();
}

void Game::executeColisao(const Chimera::KindOp &_kindOp, Chimera::Node *_pNodeA, Chimera::Node *_pNodeB) {
}
