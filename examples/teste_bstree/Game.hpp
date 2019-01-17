#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "BSPTree.h"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"

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
    void buildBuffer(int max);
    void setSquare1(std::vector<Triangle>* _pListPolygon);
    void setDrawTest(std::vector<Triangle>* _pListPolygon);
    void setDrawSplit(std::vector<Triangle>* _pListPolygon);

    BSPTree* pBspTree;
    bool isPaused;
    int botaoIndex;
    int estadoBotao;
    int debug_init;
    Chimera::TrackBall trackBall;
    Chimera::Logger* log;
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;
    unsigned int VBO, VAO; //, EBO;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
};

#endif