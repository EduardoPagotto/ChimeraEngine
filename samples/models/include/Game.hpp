#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/Engine.hpp"
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

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const { return "Game"; }

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
    Chimera::Shader shader[5];
    Chimera::Engine* engine;
};

#endif