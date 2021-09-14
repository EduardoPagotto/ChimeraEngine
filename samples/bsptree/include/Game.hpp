#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/render/Texture.hpp"
#include "chimera/render/bsp/BSPTree.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::IO::IEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void update();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    Chimera::BspTree bspTree;
    bool debugParser;

    Chimera::TrackBall trackBall;
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::TextureImg* pTex;

    Chimera::Frustum frustum;

    Chimera::SimpleRender3d render3d;
    Chimera::RenderableBsp* renderz1;
};

#endif