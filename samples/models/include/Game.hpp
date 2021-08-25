#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/NodeParticleEmitter.hpp"
#include "chimera/node/VisitorRender.hpp"
#include "chimera/physic/PhysicsControl.hpp"
#include "chimera/physic/Solid.hpp"

struct Controles {
    float yaw;
    float pitch;
    float roll;
    float throttle;
    int hat;
};

/**
 * Class Game
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Game : public Chimera::IGameClientEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Node* _pRoot, Chimera::PhysicsControl* _physicWorld);
    virtual ~Game();

    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keyCapture(SDL_Keycode tecla);
    virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb);
    virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb);
    virtual void mouseMotionCapture(SDL_MouseMotionEvent mm);
    virtual void joystickEvent(Chimera::JoystickState* pJoy);
    virtual void newFPS(const unsigned int& fps);
    virtual void windowEvent(const SDL_WindowEvent& _event);
    virtual void userEvent(const SDL_Event& _event);
    virtual bool paused();

  private:
    void updatePos();
    bool isPaused;
    int botaoIndex;
    int estadoBotao;

    std::string sPosicaoObj;
    std::string textoFPS;

    Controles crt;

    Chimera::VisitorRender renderV;
    Chimera::NodeHUD* pHUD;
    Chimera::NodeCamera* pOrbitalCam;
    Chimera::Solid* pCorpoRigido;
    Chimera::NodeParticleEmitter* pEmissor;
    Chimera::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
    Chimera::PhysicsControl* physicWorld;
};

#endif