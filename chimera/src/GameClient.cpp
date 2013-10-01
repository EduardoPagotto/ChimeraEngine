#include "GameClient.h"

namespace Chimera {

    GameClient::GameClient ( Engine3D *pEngine3D, SceneMng *_pScenMng ) : m_pEngined3D ( pEngine3D ) , m_pSceneMng ( _pScenMng )  {
    
    //m_pFont = new Chimera::Font("C:\\libs\\SDK\\freefont-ttf\\outras\\Vera.ttf",18);
    //m_pHUD = new HUD( m_pVideo->getRecTela());
    //m_pHUD->addFont(m_pFont);
    //m_pHUD->addSquare(Rectangular(10,10,500,40), Color(1.0f,1.0f, 1.0f, 0.2f));
    ////m_pHUD->addSquare(10,440,630,470,Color(0.0f,1.0f, 0.0f, 0.2f));
    //m_pHUD->addText(0,0,0,5,Color::RED,&m_sFPS);

    m_physicWorld = Singleton<PhysicWorld>::getRefSingleton();
}

GameClient::~GameClient() {
    
//     if ( m_pFont )
//         delete m_pFont;
//
//     if ( m_pHUD )
//         delete m_pHUD;

    Singleton<PhysicWorld>::releaseRefSingleton();
}

void GameClient::open() {
    start();
}

void GameClient::close ( void ) {

    SDL_Event l_eventQuit;
    l_eventQuit.type = SDL_QUIT;
    if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
        throw ExceptionSDL ( -5, std::string ( SDL_GetError() ) );
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

        m_physicWorld->stepSim();

        //Testa se ha eventos pendentes
        if ( SDL_PollEvent ( &l_eventSDL ) == 0 ) {	//Se Nao Ha eventos Pendentes execute o Render
            if ( l_isActive==true ) {			//Se nao houver foco na tela pule o render
                //m_physicWorld->stepSim();

                onFrame();

                //DataMsg dataMsg ( KindOperation::DRAW3D,nullptr,nullptr,nullptr );
                //m_pSceneMng->getRoot()->update ( &dataMsg );
                //m_pEngined3D->begin2D();
                //m_pHUD->update();
                //m_pEngined3D->end2D();
                offFrame();

                m_pEngined3D->getVideo()->swapWindow();

            } else
                SDL_WaitEvent ( NULL );

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

            DataMsg dataMsg ( KindOperation::SDL,this,&l_eventSDL,nullptr );
            processMsg ( &dataMsg );

            if(dataMsg.isDone()==false) {
                //m_pSceneMng->
            }
        }
    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
