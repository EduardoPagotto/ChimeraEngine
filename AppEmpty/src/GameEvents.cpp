#include "GameEvents.h"
#include "ExceptionSDL.h"

GameEvents::GameEvents() {}

GameEvents::~GameEvents() {}

void GameEvents::start() {}
void GameEvents::stop() {}
void GameEvents::render() {}
void GameEvents::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {}
void GameEvents::mouseButtonUpCapture ( SDL_MouseButtonEvent mb ) {}
void GameEvents::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {}
void GameEvents::joystickCapture ( Chimera::JoystickManager &joy ) {}
void GameEvents::userEvent ( const SDL_Event &_event ) {}
void GameEvents::newFPS ( const unsigned int &fps ) {}
void GameEvents::beginProcGame() {}
void GameEvents::endProcGame() {}

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
        sendMessage ( Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN,nullptr, nullptr );
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

void GameEvents::sendMessage ( Chimera::KindOp _kindOf, void *_paramA, void *_paramB ) {

    SDL_Event event;
    SDL_zero ( event );
    event.type = SDL_USEREVENT;
    event.user.code = ( int ) _kindOf;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    SDL_PushEvent ( &event );
}
