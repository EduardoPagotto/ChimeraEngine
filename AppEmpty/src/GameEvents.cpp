#include "GameEvents.h"
#include "ExceptionSDL.h"

GameEvents::GameEvents(Chimera::SceneMng *_pScenMng) : GameClient(_pScenMng) {}

GameEvents::~GameEvents() {}

void GameEvents::start() {}
void GameEvents::stop() {}
void GameEvents::render() {}
void GameEvents::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {}
void GameEvents::mouseButtonUpCapture ( SDL_MouseButtonEvent mb ) {}
void GameEvents::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {}
void GameEvents::joystickCapture ( Chimera::JoystickManager &joy ) {}

//void GameEvents::userEvent ( const SDL_Event &_event ) {}
void GameEvents::newFPS ( const unsigned int &fps ) {}
//void GameEvents::beginProcGame() {}
//void GameEvents::endProcGame() {}

void GameEvents::keyCapture ( SDL_Keycode tecla ) {

    switch ( tecla ) {
    case SDLK_ESCAPE:
        SDL_Event l_eventQuit;
        l_eventQuit.type = SDL_QUIT;
        if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
            throw Chimera::ExceptionSDL ( Chimera::ExceptionCode::CLOSE, std::string ( SDL_GetError() ) );
        }
        break;
    case SDLK_F1:
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

void GameEvents::joystickStatus ( Chimera::JoystickManager &joy ) {
    Chimera::JoystickState *joystick = joy.getJoystickState ( 0 );
    if ( joystick != nullptr ) { }
}

void GameEvents::executeColisao(const Chimera::KindOp & _kindOp, Chimera::Node * _pNodeA, Chimera::Node * _pNodeB)
{
}

