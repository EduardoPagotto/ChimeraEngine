#include "Game.h"

Game::Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng) : GameClient(_pVideo, _pScenMng) {
    
    timer01.setElapsedCount(1000);
    timer01.start();
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
    
    pSceneMng->execLight();
    
}

void Game::stop(){
}

void Game::onFrame(){
    
    using namespace Chimera;
    
    //TODO Desenho aqui
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //pEngined3D->setLight(true);
    //pEngined3D->setMaterial(true);    

    pCam = (Camera*)pSceneMng->getNode(EntityKind::CAMERA,0);
    pObj = (Object*)pSceneMng->getNode(EntityKind::OBJECT,0);
    
    const SDL_Rect *tela = pVideo->getPRectangle();
    
    pSceneMng->setViewPortPerspective( *tela, pCam);
    
    pCam->exec();
    
    Chimera::DataMsg dataMsg(KindOp::DRAW3D,this,pObj,nullptr);
    pSceneMng->update(&dataMsg);
    
    //if ( pEngined3D->getLight() ==true ) {
     //   pSceneMng->execLight();
    //} 
    
    //m_pEngined3D->setViewPortOrtogonal();
    //glColor3f(0.7, 0.5, 0.8);
    //glRectf(100.0, 100.0, 300.0, 200.0);
    
    //m_pEngined3D->setViewPortPerspective();
    //m_pEngined3D->setViewPortPerspective();
}

void Game::offFrame(){
    
    if (timer01.stepCount() == true)
        std::cout << "FPS: " << pVideo->getFPS() << std::endl;
    
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

