#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/scene/Scene.hpp"
//#include "chimera/node/NodeParticleEmitter.hpp"
#include "chimera/render/bullet/Solid.hpp"

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
    // Chimera::VisitorRender renderV;

  private:
    Chimera::Scene activeScene;
    Chimera::Renderer3d render3d;
    Chimera::Engine* engine;
    // std::string sPosicaoObj;
    // std::string textoFPS;
    Controles crt;
    // Chimera::NodeHUD* pHUD;
    // Chimera::NodeCamera* pOrbitalCam;
    Chimera::Solid* pCorpoRigido;
    // Chimera::NodeParticleEmitter* pEmissor;
    // Chimera::Node* root;
    // Chimera::Shader shader[5];
};

#endif