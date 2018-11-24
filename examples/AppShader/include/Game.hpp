#ifndef __GAME_SHADER_TEST___HPP
#define __GAME_SHADER_TEST___HPP

#include "DataMsg.hpp"
#include "IGameClientEvents.hpp"
#include "Node.hpp"
#include "Video.hpp"

#include "Shader.hpp"

class Game : public Chimera::IGameClientEvents {
  public:
    Game(Chimera::Shader* _pShader, Chimera::Video* _pVideo);

    virtual ~Game();
    // Inherited via IGameClientEvents
    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keyCapture(SDL_Keycode tecla);
    virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb);
    virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb);
    virtual void mouseMotionCapture(SDL_MouseMotionEvent mm);
    virtual void joystickCapture(Chimera::JoystickManager& joy);
    virtual void joystickStatus(Chimera::JoystickManager& joy);
    virtual void newFPS(const unsigned int& fps);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    Chimera::Video* pVideo;
    int botaoIndex;
    int estadoBotao;
    bool isPaused;

  private:
    Chimera::Shader* pShader;
    GLuint vertexbuffer;
    GLuint VertexArrayID;
};

#endif
