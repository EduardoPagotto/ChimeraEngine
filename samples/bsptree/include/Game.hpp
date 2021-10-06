#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/Texture.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/space/BSPTree.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Shader* _pShader);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void update();
    virtual void keboardEvent(SDL_Event* pEventSDL);
    virtual void mouseEvent(SDL_Event* pEventSDL);
    virtual void joystickEvent(SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    Chimera::Core::BspTree bspTree;
    Chimera::TrackBall trackBall;
    Chimera::Core::CanvasGL* pCanvas;
    Chimera::Shader* pShader;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::Texture* pTex;
    Chimera::Core::Frustum frustum;
    Chimera::SimpleRender3d render3d;
    Chimera::RenderableBsp* renderz1;
};

#endif