#include "Game.h"
#include "ExceptionSDL.h"

#include "Transform.h"
#include "OpenGLDefs.h"
#include "GameClient.h"

#include "Singleton.h"

Game::Game(Chimera::Video *_pVideo) : pVideo(_pVideo) {
	isPaused = false;

	shader =  Chimera::Singleton<Chimera::Shader>::getRefSingleton();
}

Game::~Game() {
	Chimera::Singleton<Chimera::Shader>::releaseRefSingleton();
}

void Game::joystickCapture(Chimera::JoystickManager &joy) {
}

void Game::joystickStatus(Chimera::JoystickManager &joy) {

	using namespace Chimera;
	// Captura joystick 0 se existir
	JoystickState *joystick = joy.getJoystickState(0);
	if (joystick != nullptr) {

		float deadZone = 0.5f;
		float propulsaoPrincipal = 3.0f;
		float propulsaoFrontal = 1.0f;

		float yaw = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_X, deadZone);
		float pitch = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_Y, deadZone);
		float roll = joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_X, deadZone);

		double throttle = -propulsaoPrincipal * ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_TRIGGER, deadZone)) / 2);
		throttle = throttle - (-propulsaoFrontal* ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_TRIGGER, deadZone)) / 2));

		if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::X) == true) {}
		if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::B) == true) {}

		int val = joystick->Hat(0);
		if (val & (uint8_t)JOY_PAD_COD::UP) {}
		if (val & (uint8_t)JOY_PAD_COD::DOWN) {}
		if (val & (uint8_t)JOY_PAD_COD::RIGHT) {}
		if (val & (uint8_t)JOY_PAD_COD::LEFT) {}
		if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {}
	}
}

void Game::keyCapture(SDL_Keycode tecla) {

	switch (tecla) {
	case SDLK_ESCAPE:
		SDL_Event l_eventQuit;
		l_eventQuit.type = SDL_QUIT;
		if (SDL_PushEvent(&l_eventQuit) == -1) {
			throw Chimera::ExceptionSDL(Chimera::ExceptionCode::CLOSE, std::string(SDL_GetError()));
		}
		break;
	case SDLK_F10:
		sendMessage(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
		break;
	case SDLK_F1:
	case SDLK_UP:
	case SDLK_DOWN:
	case SDLK_LEFT:
	case SDLK_RIGHT:
	case SDLK_a:
	case SDLK_s:
		break;
	default:
		break;
	}
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {
	botaoIndex = mb.button;
	estadoBotao = mb.state;
}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {
	botaoIndex = mb.button;
	estadoBotao = mb.state;
}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {

	if (estadoBotao == SDL_PRESSED) {
		if (botaoIndex == 1) {
			//pOrbitalCam->trackBall(mm.yrel, mm.xrel, 0);
		}
		else if (botaoIndex == 2) {
			//pOrbitalCam->trackBall(0, 0, mm.yrel);
		}
	}
}

void Game::start() {
	pVideo->initGL();

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
#ifdef WIN32
	shader->load("default","C:\\Projetos\\ChimeraEngine\\AppShader\\shader\\SimpleVertexShader.vertexshader",
		"C:\\Projetos\\ChimeraEngine\\AppShader\\shader\\SimpleFragmentShader.fragmentshader");
# else
	shader->load("default","/home/locutus/Projetos/ChimeraEngine/AppShader/shader/SimpleVertexShader.vertexshader",
		"/home/locutus/Projetos/ChimeraEngine/AppShader/shader/SimpleFragmentShader.fragmentshader");
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
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

}

void Game::stop() {
}

void Game::newFPS(const unsigned int &fps) {
}

void Game::userEvent(const SDL_Event & _event)
{
	Chimera::KindOp op = (Chimera::KindOp) _event.user.code;

	if (op == Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN) {
		pVideo->toggleFullScreen();
	}

}

void Game::windowEvent(const SDL_WindowEvent & _event)
{
	switch (_event.event) {
	case SDL_WINDOWEVENT_ENTER:
		isPaused = false;
		break;
	case SDL_WINDOWEVENT_LEAVE:
		isPaused = true;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		pVideo->reshape(_event.data1, _event.data2);
		break;
	default:
		break;
	}
}

bool Game::paused()
{
	return isPaused;
}

void Game::render() {

	pVideo->initDraw();
	//TODO desenhar aqui!!
	glClear(GL_COLOR_BUFFER_BIT);

	// Use our shader
	//glUseProgram(programID);
    shader->link();

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0         // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);

	pVideo->endDraw();
}
