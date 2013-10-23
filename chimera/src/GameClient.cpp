#include "GameClient.h"

namespace Chimera {

GameClient::GameClient ( Video *_pVideo, Chimera::SceneMng *_pScenMng ) : pSceneMng ( _pScenMng ), pVideo(_pVideo)  {
    
    textoFPS = "fps: " + std::to_string(0);
    
    pFont = new Chimera::Font("/usr/share/fonts/gnu-free/FreeSans.ttf",18);
    pHUD = new HUD( *pVideo->getPRectangle());
    pHUD->addFont(pFont);
    
    SDL_Rect area;
    area.x = 10;
    area.y = 10;
    area.w = 500;
    area.h = 40;
    
    pHUD->addSquare(area, Color(1.0f,1.0f, 1.0f, 0.2f));    
    pHUD->addText(0,0,0,5,Color::RED,&textoFPS);

    fps = 0;
    timerFPS.setElapsedCount ( 1000 );
    timerFPS.start();
    
    timerSegundo.setElapsedCount(1000);
    timerSegundo.start();
    
    logger = log4cxx::Logger::getLogger ( "GameClient" );
    
    physicWorld = Singleton<PhysicWorld>::getRefSingleton();
}

GameClient::~GameClient() {
    
    if ( pFont )
        delete pFont;

    if ( pHUD )
        delete pHUD;

    Singleton<PhysicWorld>::releaseRefSingleton();
}

bool GameClient::newFPS() {
    return timerSegundo.stepCount();
}

void GameClient::countFrame() {
    if ( timerFPS.stepCount() == true ) {
        fps = timerFPS.getCountStep();
        textoFPS = "fps: " + std::to_string(fps) + std::string(" Periodo: ") + std::to_string( physicWorld->getLastPeriod() );
    }   
}

void GameClient::open() {
    
    pSceneMng->initOpenGL();
    std::string l_msg =  "OpenGL iniciado com sucesso, versao: " + pSceneMng->getVersaoOpenGL();
    LOG4CXX_INFO ( logger , l_msg );  
    
    start();
    
    DataMsg dataMsg(KindOp::START,this,nullptr,nullptr);
    pSceneMng->update(&dataMsg);
    
    pHUD->setOn(true);
}

void GameClient::close ( void ) {

    SDL_Event l_eventQuit;
    l_eventQuit.type = SDL_QUIT;
    if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
        throw ExceptionSDL ( ExceptionCode::CLOSE, std::string ( SDL_GetError() ) );
    }

}

//         l_periodo = m_timerFPS.contaCiclo();
//         if ( l_periodo==true ) {
//             sprintf ( l_buffer,"FPS:%4.2f Elapse: %f RX:%f RY:%f RZ:%f",m_fps ,m_physicWorld->getTimeElapsed(),m_pScene->m_vCameras[0]->m_trans.rotation.getX(),
//                       m_pScene->m_vCameras[0]->m_trans.rotation.getY(),m_pScene->m_vCameras[0]->m_trans.rotation.getZ() );
//             m_sFPS = l_buffer;
//         }

void GameClient::gameLoop ( void ) {

    SDL_Event l_eventSDL;
    bool l_quit = false;
    bool l_isActive = false;

    while ( !l_quit ) {

        physicWorld->stepSim();

        //Testa se ha eventos pendentes
        if ( SDL_PollEvent ( &l_eventSDL ) == 0 ) {	//Se Nao Ha eventos Pendentes execute o Render
            if ( l_isActive==true ) {			//Se nao houver foco na tela pule o render
                physicWorld->stepSim();
                countFrame();
                onFrame();

                //DataMsg dataMsg ( KindOp::DRAW3D,nullptr,nullptr,nullptr );
                //m_pSceneMng->getRoot()->update ( &dataMsg );
                                
                offFrame();

                pSceneMng->begin2D();
                pHUD->update();
                pSceneMng->end2D();
                
                pVideo->swapWindow();
               
            } else
                SDL_WaitEvent ( nullptr );

        } else {

            switch ( l_eventSDL.type ) {
//                 case SDL_VIDEORESIZE:   //muda tamanho da tela
//                     m_pVideo->resizeWindow ( getEventPtr()->resize.w ,getEventPtr()->resize.h );
//                     break;
            case SDL_QUIT:
                l_quit = true;
                stop();
                break;
            case SDL_WINDOWEVENT:
                switch ( l_eventSDL.window.event ) {
                case SDL_WINDOWEVENT_ENTER:
                    l_isActive = true;
                    break;

                case SDL_WINDOWEVENT_LEAVE:
                    l_isActive = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }

            DataMsg dataMsg ( KindOp::SDL,this,&l_eventSDL,nullptr );
            processMsg ( &dataMsg );

            if(dataMsg.isDone()==false) {
                //m_pSceneMng->
            }
        }
    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
