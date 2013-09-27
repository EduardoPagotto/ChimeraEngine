#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng ( Video *_video ) : m_pVideo ( _video ) , m_quit ( false ) {

    setLight ( false );
    setMaterial ( false );
    m_pRoot = new Node ( EntityType::NODE,std::string ( "root" ) );

}

SceneMng::~SceneMng() {

}

void SceneMng::open() {

    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == 0 ) {

        m_pVideo->open();

        DataMsg dataMsg ( KindOperation::START,this,nullptr,nullptr );

        m_pRoot->update ( &dataMsg );


    } else {

        throw ExceptionSDL ( -1, std::string ( SDL_GetError() ) );

    }

}

void SceneMng::close() {

    SDL_Event l_eventQuit;
    l_eventQuit.type = SDL_QUIT;
    if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
        throw ExceptionSDL ( -4, std::string ( SDL_GetError() ) );
    }
}

// void SceneMng::run() {
// 
//     SDL_Event evento;
// 
//     while ( !m_quit ) {
// 
//         m_pVideo->execute ();
// 
//         while ( SDL_PollEvent ( &evento ) ) {
// 
//             DataMsg dataMsg ( KindOperation::SDL,this,&evento,nullptr );
//             m_pRoot->update ( &dataMsg );
// 
//             if ( evento.type == SDL_QUIT ) {
//                 m_quit = true;
// 
//             }
//         }
//     }
// 
//     m_pVideo->close();
// 
// }

void SceneMng::addChildToScene ( Node *_pNode ) {

    m_pRoot->addChild ( _pNode );

    switch ( _pNode->getType() ) {
    case EntityType::CAMERA:
        m_lCamera.push_back ( ( Camera* ) _pNode );
        break;
    case EntityType::LIGHT:
        m_lLight.push_back ( ( Light* ) _pNode );
        break;
    case EntityType::OBJECT:
        m_lObject.push_back ( ( Object* ) _pNode );
        break;
    default:
        break;
    }

}

void SceneMng::setLight ( bool _lightOn ) {

    m_hasLight = _lightOn;

    if ( _lightOn==true )
        glEnable ( GL_LIGHTING );
    else
        glDisable ( GL_LIGHTING );
}

void SceneMng::setMaterial ( bool _materialOn ) {

    m_hasMaterial = _materialOn;

    if ( _materialOn==true ) {
        glEnable ( GL_TEXTURE_2D );
        glEnable ( GL_COLOR_MATERIAL );
        glColorMaterial ( GL_FRONT,GL_DIFFUSE ); //??
    } else {
        glDisable ( GL_TEXTURE_2D );
        glDisable ( GL_COLOR_MATERIAL );
    }
}

void SceneMng::begin2D ( void ) {
    //Salva config bitŽs
    glPushAttrib ( GL_ENABLE_BIT );

    glDisable ( GL_DEPTH_TEST );
    glDisable ( GL_CULL_FACE );

    glDisable ( GL_COLOR_MATERIAL );
    glDisable ( GL_LIGHTING );

    //TODO problemas de contaminacao de cor no HUD
    //glEnable(GL_TEXTURE_2D);
    glDisable ( GL_TEXTURE_2D );

    /* This allows alpha blending of 2D textures with the scene */
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void SceneMng::end2D ( void ) {
    //Retorna para estado de maquina 3d
    glPopAttrib();

    //TODO Corrigir reaster problemas de constaminacao de cor no 3D
    glColor3f ( 1.0,1.0,1.0 );
}

// bool Engine::update ( DataMsg* dataMsg ) {
//     bool retorno = false;
//
//     if ( dataMsg->getKindOperation() == KindOperation::SDL ) {
//
//         SDL_Event *pEvento = ( SDL_Event* ) dataMsg->getParam();
//
//         switch ( pEvento->type ) {
//             case SDL_QUIT:
//                 quit = true;
//                 break;
//             case SDL_KEYDOWN:
//                 retorno = keyCaptureLocal ( pEvento->key.keysym.sym );
//                 break;
//             case SDL_MOUSEBUTTONDOWN:
//                 retorno = mouseButtonDownCaptureLocal ( pEvento->button );
//                 break;
//             case SDL_MOUSEMOTION:
//                 retorno = mouseMotionCaptureLocal ( pEvento->motion );
//                 break;
//             default:
//                 break;
//         }
//     }
//
//     return retorno;
// }

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
