#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IEvents.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/render/Texture.hpp"
#include "chimera/render/bsp/BSPTree.hpp"

class Game : public Chimera::IEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void newFPS(const unsigned int& fps);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    Chimera::BspTree bspTree;
    bool isPaused;
    bool debugParser;

    Chimera::TrackBall trackBall;
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::TextureImg* pTex;

    Chimera::Frustum frustum;
};

#endif