#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng (Node *_pRoot) : pRoot(_pRoot) {
    pRoot = _pRoot;
    parseEntity(pRoot);
}

SceneMng::~SceneMng() {

}

void SceneMng::parseEntity(Node *_pNode) {
    
    addEntityToScene( _pNode );
    
    for (Node *node : _pNode->listChild) {
        parseEntity( node );
    }
    
}

void SceneMng::addEntityToScene( Node *_pNode) {

    switch ( _pNode->getKind() ) {
        case EntityKind::CAMERA:
            m_vCamera.push_back ( ( Camera* ) _pNode );
            break;
        case EntityKind::LIGHT:
            m_vLight.push_back ( ( Light* ) _pNode );
            break;
        case EntityKind::OBJECT:
            m_vObject.push_back ( ( Object* ) _pNode );
            break;
        case EntityKind::DRAW: 
            m_vDraw.push_back( ( Draw* ) _pNode );
            break;
        case EntityKind::IMAGE: 
            m_vImage.push_back( ( Image* ) _pNode );
            break;
        case EntityKind::MATERIAL: 
            m_vMaterial.push_back( ( Material* ) _pNode );
            break;
        case EntityKind::EFFECT: 
            m_vEffect.push_back( ( Effect* ) _pNode );
            break;
        default:
            break;
    }
    
}

void SceneMng::addChildToScene ( Node *_pNode ) {

    pRoot->addChild ( _pNode );
    addEntityToScene( _pNode );
    
}

Node *SceneMng::getNode ( EntityKind _type, int index ) {

    Node *retono = nullptr;

    switch ( _type ) {
    case EntityKind::CAMERA:
        if ( m_vCamera.size() > index )
            retono = m_vCamera[index];
        break;
    case EntityKind::LIGHT:
        if ( m_vLight.size() > index )
            retono = m_vLight[index];
        break;
    case EntityKind::OBJECT:
        if ( m_vObject.size() > index )
            retono = m_vObject[index];
        break;
    default:
        break;
    }

    return retono;
}

void SceneMng::execLight() {
    for ( Light *pLight : m_vLight ) {
        pLight->exec();
    }
}

void SceneMng::update(DataMsg *dataMsg ) {
    
    pRoot->update ( dataMsg );
        
}

void SceneMng::initOpenGL(Video *_pVideo) {
        
    SDL_GL_SetSwapInterval ( 1 );
    
    /* Enable Texture Mapping ( NEW ) */
    glEnable( GL_TEXTURE_2D );
    
    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );
    
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    
    /* Depth buffer setup */
    glClearDepth( 1.0f );
    
    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );
    
    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );
    
    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );  
    
    
    //TODO AQUII!!!!!
    //     glShadeModel ( GL_SMOOTH );
    //     glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    //     glClearDepth ( 1.0f );
    //     glEnable ( GL_DEPTH_TEST );
    //     glEnable ( GL_CULL_FACE );
    //     glDepthFunc ( GL_LEQUAL );
    //     glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    //     
    //     setLight ( false );
    //     setMaterial ( true );
}

std::string SceneMng::getVersaoOpenGL() {
    
    std::string retorno;
    
    const char* version = (const char*)glGetString ( GL_VERSION );
    if ( version != nullptr ) {
        retorno.append(version);
    } else {
        throw ExceptionSDL ( ExceptionCode::READ, std::string ( SDL_GetError() ) ); 
    }
    
    return retorno;
}

void SceneMng::setViewPortOrtogonal(SDL_Rect _rectangle) {
    
    glViewport ( _rectangle.x , _rectangle.y , _rectangle.w, _rectangle.h );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    glOrtho ( _rectangle.x, _rectangle.w, _rectangle.y ,_rectangle.h,-1.0,1.0 );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    
}

void SceneMng::setViewPortPerspective(SDL_Rect _rectangle, Camera *_camera) {
    
    glViewport ( _rectangle.x, _rectangle.y, _rectangle.w, _rectangle.h );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective (_camera->getFov(), ( GLfloat ) ( float ) _rectangle.w / ( float ) _rectangle.h, _camera->getNear(), _camera->getFar() );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    
}

// void Engine3D::setLight ( bool _lightOn ) {
//     
//     hasLight = _lightOn;
//     
//     if ( _lightOn==true )
//         glEnable ( GL_LIGHTING );
//     else
//         glDisable ( GL_LIGHTING );
// }
// 
// void Engine3D::setMaterial ( bool _materialOn ) {
//     
//     hasMaterial = _materialOn;
//     
//     if ( _materialOn==true ) {
//         glEnable ( GL_TEXTURE_2D );
//        //glEnable ( GL_COLOR_MATERIAL );
//        // glColorMaterial ( GL_FRONT, GL_DIFFUSE ); //??
//     } else {
//         glDisable ( GL_TEXTURE_2D );
//         glDisable ( GL_COLOR_MATERIAL );
//     }
// }
// 
// void Engine3D::begin2D () {
//     //Salva config bitŽs
//     glPushAttrib ( GL_ENABLE_BIT );
//     
//     glDisable ( GL_DEPTH_TEST );
//     glDisable ( GL_CULL_FACE );
//     
//     glDisable ( GL_COLOR_MATERIAL );
//     glDisable ( GL_LIGHTING );
//     
//     //TODO problemas de contaminacao de cor no HUD
//     //glEnable(GL_TEXTURE_2D);
//     glDisable ( GL_TEXTURE_2D );
//     
//     /* This allows alpha blending of 2D textures with the scene */
//     glEnable ( GL_BLEND );
//     glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
// }
// 
// void Engine3D::end2D ( void ) {
//     //Retorna para estado de maquina 3d
//     glPopAttrib();
//     
//     //TODO Corrigir reaster problemas de constaminacao de cor no 3D
//     glColor3f ( 1.0,1.0,1.0 );
// }

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
