#include "Game.h"

Game::Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng) : GameClient(_pVideo, _pScenMng) {
    
    timer01.setElapsedCount(1000);
    timer01.start();
    
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
    
    const SDL_Rect *tela = pVideo->getPRectangle();
    pSceneMng->setViewPortPerspective( *tela, pCam);
    
    pSceneMng->execLight();
    
    xrot = 0.0;
    yrot = 0.0;
}

void Game::stop(){
}

void Game::onFrame(){
    
    using namespace Chimera;
   
    //TODO Desenho aqui
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
    //pCam->exec();
    
    /* Reset the view */
    glLoadIdentity( );
    
    /* Translate Into/Out Of The Screen By z */
    glTranslatef( 0.0f, 0.0f, -5.0 );
    
    glRotatef( xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis By xrot */
    glRotatef( yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis By yrot */
    
   
    Chimera::DataMsg dataMsg(KindOp::DRAW3D,this,pObj,nullptr);
    pSceneMng->update(&dataMsg);
    
    xrot += 1.0f;
    yrot += 1.0f;
    
    //pSceneMng->execLight();
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
        std::cout << "FPS: " << getFPS() << std::endl;
    
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

