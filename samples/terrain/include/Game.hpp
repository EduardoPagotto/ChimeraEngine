#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/render/HeightMap.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/VertexStatic.hpp"

class Game : public Chimera::IGameClientEvents {
  public:
    Game();
    virtual ~Game();
    // Inherited via IGameClientEvents
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
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    bool isPaused;
    int botaoIndex;
    int estadoBotao;
    bool debugParser;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::TrackBall trackBall;
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;
    // Chimera::Light* pLight;
    Chimera::Material* pMaterial;
    Chimera::VertexRenderStatic renderStat;

    Chimera::HeightMap* pHeightMap;
};

#endif