#include "GameClient.h"

namespace Chimera {

GameClient::GameClient ( Video *_pVideo, SceneMng *_pScenMng ) : m_pVideo ( _pVideo ) , m_pSceneMng ( _pScenMng ) {
    m_status = false;
    m_isActive = false;
    //m_fps = 0.0f;

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

bool GameClient::init ( void ) {
    m_status=true;
    m_isActive = true;
    //m_timerFPS.setCiclo ( 1000 );

    //m_pScene->init();

    start();

    return true;
}

// void GameClient::done(void)
// {
// 	SDL_Event l_eventQuit;
// 	l_eventQuit.type = SDL_QUIT;
// 	if(SDL_PushEvent(&l_eventQuit) == -1)
// 	{
// 		CHIMERA_THROW(99,SDL_GetError());
// 	}
//
// 	m_status = false;
// 	m_isActive = false;
//
// 	stopApp();
// }

void GameClient::gameLoop ( void ) {

    //bool l_periodo = false;
    //char l_buffer[256];
    SDL_Event l_eventSDL;

    while ( m_status ) {
//         l_periodo = m_timerFPS.contaCiclo();
//         if ( l_periodo==true ) {
//             sprintf ( l_buffer,"FPS:%4.2f Elapse: %f RX:%f RY:%f RZ:%f",m_fps ,m_physicWorld->getTimeElapsed(),m_pScene->m_vCameras[0]->m_trans.rotation.getX(),
//                       m_pScene->m_vCameras[0]->m_trans.rotation.getY(),m_pScene->m_vCameras[0]->m_trans.rotation.getZ() );
//             m_sFPS = l_buffer;
//         }

        m_physicWorld->stepSim();

        //Testa se ha eventos pendentes
        if ( SDL_PollEvent ( &l_eventSDL ) == 0 ) {	//Se Nao Ha eventos Pendentes execute o Render
            if ( m_isActive==true ) {			//Se nao houver foco na tela pule o render
                //m_physicWorld->stepSim();

                m_pVideo->execute();

                onFrame();

                DataMsg dataMsg ( KindOperation::DRAW3D,nullptr,nullptr,nullptr );


                m_pSceneMng->getRoot()->update ( &dataMsg );

                //m_fps = m_timerFPS.getCiclo();

                m_pSceneMng->begin2D();

                //m_pHUD->update();

                m_pSceneMng->end2D();


                offFrame();

                m_pVideo->swapWindow();

            } else
                SDL_WaitEvent ( NULL );

        } else {

            switch ( l_eventSDL.type ) {
//                 case SDL_VIDEORESIZE:   //muda tamanho da tela
//                     m_pVideo->resizeWindow ( getEventPtr()->resize.w ,getEventPtr()->resize.h );
//                     break;
            case SDL_KEYDOWN:
                keyCapture ( l_eventSDL.key.keysym.sym );
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseButtonDownCapture( l_eventSDL.button );
                break;
            case SDL_MOUSEMOTION:
                mouseMotionCapture ( l_eventSDL.motion );
                break;
//             case SDL_ACTIVEEVENT:   //Lost Focus, dont draw
//                 m_isActive = ( getEventPtr()->active.gain == 0 ? false : true );
//                 break;
            default:
//                     pullEvent ( NULL );
                break;
            }
        }
    }
    SDL_Quit();
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
