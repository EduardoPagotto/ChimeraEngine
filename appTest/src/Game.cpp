#include "Game.h"

Game::Game(Chimera::Engine3D *pEngine3D, Chimera::SceneMng *_pScenMng):GameClient(pEngine3D,_pScenMng) {
}

Game::~Game() {
}

bool Game::keyCapture ( SDL_Keycode tecla ) {

    bool retorno = false;

    switch ( tecla ) {
    case SDLK_ESCAPE:
        GameClient::close();
        retorno = true;
        break;
    default:
        break;
    }

    return retorno;

}

bool Game::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {
    return false;
}

bool Game::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {
    return false;
}

void Game::start() {
}

void Game::stop(){
}

void Game::onFrame(){
    
    //TODO Desenho aqui
    glColor3f(0.7, 0.5, 0.8);
    glRectf(1.0, 1.0, 3.0, 2.0);
    
}

void Game::offFrame(){
}

void Game::processMsg(Chimera::DataMsg *dataMsg) {
    
    if ( dataMsg->getKindOperation() == Chimera::KindOperation::SDL ) {
        
        bool encerraParse = false;
        SDL_Event *pEvento = ( SDL_Event* ) dataMsg->getParam();
        
        switch ( pEvento->type ) {
            case SDL_QUIT:
                break;
            case SDL_KEYDOWN:
                encerraParse = keyCapture ( pEvento->key.keysym.sym );
                break;
            case SDL_MOUSEBUTTONDOWN:
                encerraParse = mouseButtonDownCapture ( pEvento->button );
                break;
            case SDL_MOUSEMOTION:
                encerraParse = mouseMotionCapture ( pEvento->motion );
                break;
            default:
                break;
        }
       
        dataMsg->setDone(encerraParse); 
    }   
}

