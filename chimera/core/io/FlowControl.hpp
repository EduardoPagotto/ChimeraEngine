#ifndef __CHIMERA_CORE_FLOW_CONTROL__HPP
#define __CHIMERA_CORE_FLOW_CONTROL__HPP

#include "IEvents.hpp"
#include "LayerStack.hpp"
#include "Timer.hpp"

namespace Chimera {

class FlowControl {
  public:
    FlowControl(IEvents* _gEvent);
    virtual ~FlowControl();
    void run();

  private:
    bool changeStatusFlow(SDL_Event* pEventSDL);
    LayerStack layerStack;
    IEvents* gEvent;
    Timer timerFPS;
    uint32_t fps;
    bool pause;
};
} // namespace Chimera

#endif