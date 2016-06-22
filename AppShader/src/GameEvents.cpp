#include "GameEvents.h"
#include "ExceptionSDL.h"
#include "Shader.h"

GameEvents::GameEvents() {}

GameEvents::~GameEvents() {

    // Cleanup VBO
    glDeleteBuffers ( 1, &vertexbuffer );
    glDeleteVertexArrays ( 1, &VertexArrayID );
}

void GameEvents::start() {

    // Dark blue background
    glClearColor ( 0.0f, 0.0f, 0.4f, 0.0f );

    glGenVertexArrays ( 1, &VertexArrayID );
    glBindVertexArray ( VertexArrayID );

    // Create and compile our GLSL program from the shaders
#ifdef WIN32
    programID = Chimera::LoadShaders ( "C:\\Projetos\\ChimeraEngine\\AppShader\\shader\\SimpleVertexShader.vertexshader",
                                       "C:\\Projetos\\ChimeraEngine\\AppShader\\shader\\SimpleFragmentShader.fragmentshader" );
# else
    programID = Chimera::LoadShaders ( "/home/locutus/Projetos/ChimeraEngine/AppShader/shader/SimpleVertexShader.vertexshader",
                                       "/home/locutus/Projetos/ChimeraEngine/AppShader/shader/SimpleFragmentShader.fragmentshader" );
# endif

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    //GLuint vertexbuffer;

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers ( 1, &vertexbuffer );

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer ( GL_ARRAY_BUFFER, vertexbuffer );

    // Give our vertices to OpenGL.
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );

}

void GameEvents::stop() {

}

void GameEvents::render() {

    // Clear the screen
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram ( programID );

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray ( 0 );
    glBindBuffer ( GL_ARRAY_BUFFER, vertexbuffer );
    glVertexAttribPointer (
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        ( void* ) 0         // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays ( GL_TRIANGLES, 0, 3 ); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray ( 0 );

}

void GameEvents::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {}

void GameEvents::mouseButtonUpCapture ( SDL_MouseButtonEvent mb ) {}

void GameEvents::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {}

void GameEvents::joystickCapture ( Chimera::Device::JoystickManager &joy ) {}

void GameEvents::userEvent ( const SDL_Event &_event ) {}

void GameEvents::newFPS ( const unsigned int &fps ) {}

void GameEvents::beginProcGame() {}

void GameEvents::endProcGame() {}

void GameEvents::keyCapture ( SDL_Keycode tecla ) {

    switch ( tecla ) {
    case SDLK_ESCAPE:
        SDL_Event l_eventQuit;
        l_eventQuit.type = SDL_QUIT;
        if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
            throw Chimera::ExceptionSDL ( Chimera::ExceptionCode::CLOSE, std::string ( SDL_GetError() ) );
        }
        break;
    case SDLK_F1:
        break;
    case SDLK_F10:
        sendMessage ( Chimera::Graph::KindOp::VIDEO_TOGGLE_FULL_SCREEN,nullptr, nullptr );
        break;
    case SDLK_UP:
        break;
    case SDLK_DOWN:
        break;
    case SDLK_LEFT:
        break;
    case SDLK_RIGHT:
        break;
    case SDLK_a:
        break;
    case SDLK_s:
        break;
    default:
        break;
    }

}

void GameEvents::joystickStatus ( Chimera::Device::JoystickManager &joy ) {
    Chimera::Device::JoystickState *joystick = joy.getJoystickState ( 0 );
    if ( joystick != nullptr ) { }
}

void GameEvents::sendMessage ( Chimera::Graph::KindOp _kindOf, void *_paramA, void *_paramB ) {

    SDL_Event event;
    SDL_zero ( event );
    event.type = SDL_USEREVENT;
    event.user.code = ( int ) _kindOf;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    SDL_PushEvent ( &event );

}
