#include "Engine3D.h"

namespace Chimera {

Engine3D::Engine3D() : m_hasLight(false), m_hasMaterial(false) {
    
    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == 0 ) {
        
        // set the opengl context version
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
        
        // turn on double buffering set the depth buffer to 24 bits
        // you may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
        SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );
        
    } else {
        throw ExceptionSDL ( -1, std::string ( SDL_GetError() ) );
    }   
    
}

Engine3D::~Engine3D() {
}

void Engine3D::setViewPortOrtogonal() {
    setViewPortOrtogonal(0,0,m_pVideo->getWidth(), m_pVideo->getHeight());   
}

void Engine3D::setViewPortOrtogonal(int posInitX, int posIntY, int width, int height) {
    
    glViewport ( posInitX , posIntY , width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    glOrtho ( posInitX, width, posIntY, height,-1.0,1.0 );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    
}

void Engine3D::setViewPortPerspective(float near, float far, float fov) {
    setViewPortPerspective(0,0,m_pVideo->getWidth(), m_pVideo->getHeight(),near,far,fov);
}

void Engine3D::setViewPortPerspective(int posInitX, int posIntY, int width, int height, float near, float far, float fov) {
    
    glViewport ( posInitX , posIntY , width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective (fov, ( GLfloat ) ( float ) width / ( float ) height, near, far );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    
}

void Engine3D::setViewPortPerspective(Camera *_camera) {
    
    glViewport ( 0,0,m_pVideo->getWidth(), m_pVideo->getHeight() );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective (_camera->getFov(), ( GLfloat ) ( float ) m_pVideo->getWidth() / ( float ) m_pVideo->getHeight(), _camera->getNear(), _camera->getFar() );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    
}

void Engine3D::initOpenGL(Video *pVideo) {
    
    m_pVideo = pVideo;
    
    SDL_GL_SetSwapInterval ( 1 );
    
    glShadeModel ( GL_SMOOTH );
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth ( 1.0f );
    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_CULL_FACE );
    glDepthFunc ( GL_LEQUAL );
    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    
    setLight ( false );
    setMaterial ( false );
}

std::string Engine3D::getVersaoOpenGL() {
    
    std::string retorno;
       
    const char* version = (const char*)glGetString ( GL_VERSION );
    if ( version != NULL ) {
       retorno.append(version);
    } else {
        throw ExceptionSDL ( -1, std::string ( SDL_GetError() ) ); 
    }
       
    return retorno;
}

void Engine3D::setLight ( bool _lightOn ) {
    
    m_hasLight = _lightOn;
    
    if ( _lightOn==true )
        glEnable ( GL_LIGHTING );
    else
        glDisable ( GL_LIGHTING );
}

void Engine3D::setMaterial ( bool _materialOn ) {
    
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

void Engine3D::begin2D ( void ) {
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

void Engine3D::end2D ( void ) {
    //Retorna para estado de maquina 3d
    glPopAttrib();
    
    //TODO Corrigir reaster problemas de constaminacao de cor no 3D
    glColor3f ( 1.0,1.0,1.0 );
}

}