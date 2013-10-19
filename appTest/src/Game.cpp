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
    case SDLK_UP:
        pObj->applyForce( btVector3(10.0, 0.0, 0.0) );
        break;
    case SDLK_DOWN:
        pObj->applyForce( btVector3(-10.0, 0.0, 0.0) );
        break;
    case SDLK_LEFT:
        pObj->applyForce( btVector3(0.0, 10.0, 0.0) );
        break;
    case SDLK_RIGHT:
        pObj->applyForce( btVector3(0.0, -10.0, 0.0) );
        break;
    case SDLK_MINUS:
        pObj->applyTorc( btVector3(0.0, -10.0, 0.0) );
        break;
    case SDLK_PLUS:
        pObj->applyTorc( btVector3(0.0, 10.0, 0.0) );
        break;
    default:
        break;
    }

    return retorno;

}

bool Game::mouseButtonUpCapture ( SDL_MouseButtonEvent mb ) {
    
    botaoIndex = mb.button;
    estadoBotao = mb.state;
    
}

bool Game::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {
    
    botaoIndex = mb.button;
    estadoBotao = mb.state;
    
    return false;
}

bool Game::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {
    
    if (estadoBotao == SDL_PRESSED) {
        if (botaoIndex == 1) {
            pOrbitalCam->trackBall( mm.yrel, mm.xrel , 0);
        } else if (botaoIndex == 2) {
            pOrbitalCam->trackBall( 0, 0 , mm.yrel);
        } 
    } else {
           
    }
    
    
    return false;
}

void Game::start() {
    
    using namespace Chimera;
        
    physicWorld->setGravity( btVector3(0.0f, 0.0f, 0.0f) );
    
    //instancia e coloca na cena uma nova camera orbital
    Camera* pCam = (Camera*)pSceneMng->getNode(EntityKind::CAMERA,0);
    pOrbitalCam = new CameraSpherical( *pCam );
    pOrbitalCam->setDistanciaMaxima(1000.0f);
    pOrbitalCam->setDistanciaMinima(0.5f);
    pOrbitalCam->setId("Orbital1");
    pSceneMng->addChildToScene(pOrbitalCam);
    
    //Ajusta objeto primario
    pObj = (Object*)pSceneMng->getNode(EntityKind::OBJECT,0);
    pSceneMng->setLight(true);
    pSceneMng->setMaterial(true);
    
//     //define propriedades fisicas
//     btMaterial *pMat = new btMaterial;
//     pMat->m_friction = 0.1f;
//     pMat->m_restitution =0.1f;
//     
//     Physics *pPhysc = new Physics;
//     pPhysc->setMass(10.0f);
//     pPhysc->m_pPhysicMaterial = pMat;
//     
//     pObj->pPhysic = pPhysc;
//     pPhysc->setShapeBox( pObj->pDraw->getSizeBox() );
    
}

void Game::stop(){
}

void Game::onFrame(){
    
    using namespace Chimera;
   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
    const SDL_Rect *tela = pVideo->getPRectangle();
    pSceneMng->setViewPortPerspective( *tela, pOrbitalCam);
    
    
    
    pOrbitalCam->exec();
        
   
    Chimera::DataMsg dataMsg(KindOp::DRAW3D,this,pObj,nullptr);
    pSceneMng->update(&dataMsg);
        
    pSceneMng->execLight();
    
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
            case SDL_MOUSEBUTTONUP:    
                encerraParse = mouseButtonUpCapture( pEvento->button );
            case SDL_MOUSEMOTION:
                encerraParse = mouseMotionCapture ( pEvento->motion );
                break;
            default:
                break;
        }
       
        dataMsg->setDone(encerraParse); 
    }   
}

