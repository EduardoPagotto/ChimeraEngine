#ifndef __GAME_TESTE_TERRAIN__HPP
#define __GAME_TESTE_TERRAIN__HPP

#include "Camera.hpp"
#include "Terrain.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/core/Shader.hpp"

class Game : public Chimera::IGameClientEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader);

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
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;
    void RenderShadowMap();
    void CheckCameraTerrainPosition(glm::vec3& Movement);
    int LastX, LastY, LastClickedX, LastClickedY;
    ChimeraNew::Camera camera;
    Chimera::Terrain terra;
    float LightAngle;
    glm::mat4 LightViewMatrix, LightProjectionMatrix, ShadowMatrix;
    int ShadowMapSize;
    GLuint ShadowMap, RotationTexture, FBO;
    bool Wireframe, DisplayShadowMap, RenderAABB, VisualizeRenderingOrder, SortVisibleGeometryNodes, RenderSlow;
    int Depth;
    GLuint *UniformLocations, *AttribLocations;

    // Chimera::TexImg* pTex;
};

#endif