#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/Application.hpp"
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

class Game : public Chimera::Application {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();

    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

    Chimera::VisitorRender renderV;

  private:
    void updatePos();
    std::string sPosicaoObj;
    std::string textoFPS;

    Controles crt;

    Chimera::NodeHUD* pHUD;
    Chimera::NodeCamera* pOrbitalCam;
    Chimera::Solid* pCorpoRigido;
    Chimera::NodeParticleEmitter* pEmissor;
    Chimera::Node* root;
    Chimera::PhysicsControl* physicWorld;
};

#endif