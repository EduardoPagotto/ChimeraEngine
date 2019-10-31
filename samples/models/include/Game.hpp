#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/RenderVisitor.hpp"
#include "chimera/physic/PhysicsControl.hpp"
#include "chimera/physic/Solid.hpp"

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
    virtual void joystickCapture(Chimera::JoystickManager& joy);
    virtual void joystickStatus(Chimera::JoystickManager& joy);
    virtual void newFPS(const unsigned int& fps);
    virtual void windowEvent(const SDL_WindowEvent& _event);
    virtual void userEvent(const SDL_Event& _event);
    virtual bool paused();

  private:
    bool isPaused;
    int botaoIndex;
    int estadoBotao;

    std::string sPosicaoObj;
    std::string textoFPS;

    Chimera::RenderVisitor renderV;
    Chimera::HUD* pHUD;
    Chimera::Camera* pOrbitalCam;
    Chimera::Solid* pCorpoRigido;
    Chimera::ParticleEmitter* pEmissor;
    Chimera::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
    Chimera::PhysicsControl* physicWorld;
};

#endif