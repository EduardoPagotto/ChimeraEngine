#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/DataMsg.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/node/CameraSpherical.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/PhysicsControl.hpp"
#include "chimera/node/SceneMng.hpp"
#include "chimera/node/Solid.hpp"

#include <spdlog/spdlog.h>

/**
 * Class Game
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Game : public Chimera::IGameClientEvents {
  public:
    Game(Chimera::SceneMng* _pScenMng, Chimera::Video* _pVideo, Chimera::PhysicsControl* _physicWorld);
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
    int botaoIndex;
    int estadoBotao;

    Chimera::HUD* pHUD;

    std::string sPosicaoObj;
    std::string textoFPS;

    Chimera::CameraSpherical* pOrbitalCam;
    Chimera::Solid* pCorpoRigido;
    Chimera::ParticleEmitter* pEmissor;

    Chimera::SceneMng* pSceneMng;
    Chimera::PhysicsControl* physicWorld;

    bool isPaused;

    Chimera::Video* pVideo;

    std::shared_ptr<spdlog::logger> log;
};

#endif