#pragma once

//#include "LayerStack.hpp"
#include "Canvas.hpp"
#include "Timer.hpp"

namespace Chimera {

class Application {
  public:
    Application(Canvas* canvas);
    virtual ~Application();
    void run();

    int eyeIndice;

  protected:
    // void pushLayer(ILayer* layer);
    // void pushOverlay(ILayer* overlay);

    Canvas* canvas;

  private:
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;

    bool changeStatusFlow(SDL_Event* pEventSDL);
    // LayerStack layerStack;
    Timer timerFPS;
    uint32_t fps;
    bool pause;
};
} // namespace Chimera
