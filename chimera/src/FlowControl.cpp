#include "FlowControl.h"
#include "ExceptionSDL.h"
#include "DataMsg.h"
#include <iostream>

namespace Chimera {

FlowControl::FlowControl ( Device::Video *_pVideo, IGameClientEvents *_pGameClientEvents ) : pVideo ( _pVideo ), pGameClientEvents ( _pGameClientEvents ) {
    timerFPS.setElapsedCount ( 1000 );
    timerFPS.start();
}

FlowControl::~FlowControl() {

}

void FlowControl::open() {

    joystickManager.Initialize();
    joystickManager.FindJoysticks();
    std::cout << "Joystick: " << joystickManager.GetStatusManager() << std::endl;

    pVideo->initGL();

    pGameClientEvents->start();
}

void FlowControl::close ( void ) {

    SDL_Event l_eventQuit;
    l_eventQuit.type = SDL_QUIT;
    if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
        throw ExceptionSDL ( ExceptionCode::CLOSE, std::string ( SDL_GetError() ) );
    }
}

void FlowControl::countFrame() {
    if ( timerFPS.stepCount() == true ) {
        unsigned int fps = timerFPS.getCountStep();
        pGameClientEvents->newFPS ( fps );
    }
}

void FlowControl::processaGame() {

    pGameClientEvents->beginProcGame();

    countFrame();
    pVideo->initDraw();

    pGameClientEvents->render();

    pVideo->endDraw();

    pGameClientEvents->joystickStatus ( joystickManager );

    pGameClientEvents->endProcGame();
}

void FlowControl::gameLoop ( void ) {

    SDL_Event l_eventSDL;
    bool l_quit = false;
    bool l_isActive = false;

    while ( !l_quit ) {

        while ( SDL_PollEvent ( &l_eventSDL ) ) {

            switch ( l_eventSDL.type ) {
            case SDL_USEREVENT:

                if ( ( Graph::KindOp ) l_eventSDL.user.code == Graph::KindOp::VIDEO_TOGGLE_FULL_SCREEN ) {
                    pVideo->toggleFullScreen();
                } else {
                    pGameClientEvents->userEvent ( l_eventSDL );
                }

                break;
            case SDL_KEYDOWN:
                pGameClientEvents->keyCapture ( l_eventSDL.key.keysym.sym );
                break;
            case SDL_MOUSEBUTTONDOWN:
                pGameClientEvents->mouseButtonDownCapture ( l_eventSDL.button );
                break;
            case SDL_MOUSEBUTTONUP:
                pGameClientEvents->mouseButtonUpCapture ( l_eventSDL.button );
                break;
            case SDL_MOUSEMOTION:
                pGameClientEvents->mouseMotionCapture ( l_eventSDL.motion );
                break;
            case SDL_QUIT:
                l_quit = true;
                pGameClientEvents->stop();
                break;
            case SDL_WINDOWEVENT: {
                switch ( l_eventSDL.window.event ) {
                case SDL_WINDOWEVENT_ENTER:
                    l_isActive = true;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    l_isActive = false;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    pVideo->reshape ( l_eventSDL.window.data1, l_eventSDL.window.data2 );
                    break;
                default:
                    break;
                }
            }
            break;
            default:
                if ( joystickManager.TrackEvent ( &l_eventSDL ) == true ) {
                    pGameClientEvents->joystickCapture ( joystickManager );
                }
                break;
            }
        }

        //if (l_isActive==true) {
        //Se nao houver foco na tela pule o render
        processaGame();
        //}
    }
}
}