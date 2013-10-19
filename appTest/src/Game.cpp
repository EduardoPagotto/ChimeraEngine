#include "Game.h"

Game::Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng) : GameClient(_pVideo, _pScenMng) {
       
    logger = log4cxx::Logger::getLogger ( "Game" );
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
    
    using namespace Chimera;
        
    pCam = (Camera*)pSceneMng->getNode(EntityKind::CAMERA,0);
    pObj = (Object*)pSceneMng->getNode(EntityKind::OBJECT,0);
    pSceneMng->setLight(true);
    pSceneMng->setMaterial(true);
    
}

void Game::stop(){
}

void Game::onFrame(){
    
    using namespace Chimera;
   
    //TODO Desenho aqui
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
    const SDL_Rect *tela = pVideo->getPRectangle();
    pSceneMng->setViewPortPerspective( *tela, pCam);
    
    pSceneMng->execLight();
    
    pCam->exec();
        
   
    Chimera::DataMsg dataMsg(KindOp::DRAW3D,this,pObj,nullptr);
    pSceneMng->update(&dataMsg);
        
}

void Game::offFrame(){
    
    if (newFPS() == true)
        std::cout << "FPS: " << getFps() << std::endl;
    
}

void Game::processMsg(Chimera::DataMsg *dataMsg) {
    
    if ( dataMsg->getKindOp() == Chimera::KindOp::SDL ) {
        
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

