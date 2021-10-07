#ifndef __CHIMERA_APPLICATION_CONTROLLER__HPP
#define __CHIMERA_APPLICATION_CONTROLLER__HPP

#include "chimera/core/io/LayerStack.hpp"
#include "chimera/core/io/Timer.hpp"

namespace Chimera {

class Application {
  public:
    Application();
    virtual ~Application();
    void run();

  private:
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;

    bool changeStatusFlow(SDL_Event* pEventSDL);
    LayerStack layerStack;
    Timer timerFPS;
    uint32_t fps;
    bool pause;
};
} // namespace Chimera

#endif