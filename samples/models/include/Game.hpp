#ifndef __GAME_XX__HPP
#define __GAME_XX__HPP

#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
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
class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Node* _pRoot, Chimera::PhysicsControl* _physicWorld);
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
    Chimera::Core::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
    Chimera::PhysicsControl* physicWorld;
};

#endif