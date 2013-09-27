#include "Game.h"

Game::Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng):GameClient(_pVideo,_pScenMng) {

}

Game::~Game() {
    
}

void Game::keyCapture ( SDL_Keycode tecla ) {

    //bool retorno = false;

    switch ( tecla ) {
    case SDLK_ESCAPE:
        SDL_Event l_eventQuit;
        l_eventQuit.type = SDL_QUIT;
        if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
            throw Chimera::ExceptionSDL ( -4, std::string ( SDL_GetError() ) );
        }
        break;
    default:
        break;
    }

    //return retorno;

}

void Game::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {

}

void Game::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {

}

void Game::start() {
}

void Game::stop(){
}

void Game::onFrame(){
    
   
    
        //TODO Desenho aqui
    //glColor3f(0.7, 0.5, 0.8);
    //glRectf(1.0, 1.0, 3.0, 2.0);
    
}

void Game::offFrame(){
}

// void Game::update ( Chimera::DataMsg* dataMsg ) {
//     
//     if ( dataMsg->getKindOperation() == Chimera::KindOperation::SDL ) {
//         
//         SDL_Event *pEvento = ( SDL_Event* ) dataMsg->getParam();
//         
//         switch ( pEvento->type ) {
//             case SDL_QUIT:
//                 break;
//             case SDL_KEYDOWN:
//                 keyCapture ( pEvento->key.keysym.sym );
//                 break;
//             case SDL_MOUSEBUTTONDOWN:
//                 mouseButtonDownCapture ( pEvento->button );
//                 break;
//             case SDL_MOUSEMOTION:
//                 mouseMotionCapture ( pEvento->motion );
//                 break;
//             default:
//                 break;
//         }
//     }
//  
//     SceneMng::update(dataMsg);
//     
// }

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
