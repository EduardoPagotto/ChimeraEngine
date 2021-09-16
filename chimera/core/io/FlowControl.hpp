#ifndef __CHIMERA_CORE_FLOW_CONTROL__HPP
#define __CHIMERA_CORE_FLOW_CONTROL__HPP

#include "IEvents.hpp"
#include "JoystickManager.hpp"
#include "Timer.hpp"

namespace Chimera::IO {

class FlowControl {
  public:
    FlowControl(IEvents* _gEvent);
    virtual ~FlowControl();
    void run();

  private:
    bool changeStatusFlow(SDL_Event* pEventSDL);

    IEvents* gEvent;
    JoystickManager joystickManager;
    MouseDevice mouse;
    Timer timerFPS;
    uint32_t fps;
    bool pause;
};
} // namespace Chimera::IO

#endif