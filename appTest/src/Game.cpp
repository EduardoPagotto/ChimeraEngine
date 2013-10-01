#include "Game.h"

Game::Game(Chimera::Engine3D *pEngine3D, Chimera::SceneMng *_pScenMng):GameClient(pEngine3D,_pScenMng) {
    
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
    
    //m_pSceneMng-
    
}

void Game::stop(){
}

void Game::onFrame(){
    
    using namespace Chimera;
    
    //TODO Desenho aqui
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_pEngined3D->setLight(true);
    m_pEngined3D->setMaterial(true);    

    pCam = (Camera*)m_pSceneMng->getNode(EntityType::CAMERA,0);
    pObj = (Object*)m_pSceneMng->getNode(EntityType::OBJECT,0);
    
    m_pEngined3D->setViewPortPerspective(pCam);
    
    pCam->exec();
    
    Chimera::DataMsg dataMsg(KindOperation::DRAW3D,this,pObj,nullptr);
    
    m_pSceneMng->execute(&dataMsg);
    
    //m_pEngined3D->setViewPortOrtogonal();
    //glColor3f(0.7, 0.5, 0.8);
    //glRectf(100.0, 100.0, 300.0, 200.0);
    
    //m_pEngined3D->setViewPortPerspective();
    
    
    
    //m_pEngined3D->setViewPortPerspective();
    
    
}

void Game::offFrame(){
    
    //if (timer01.stepCount() == true)
        std::cout << "FPS: " << m_pEngined3D->getVideo()->getFPS() << std::endl;
    
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

