#ifndef __CHIMERA_CORE_FLOW_CONTROL__HPP
#define __CHIMERA_CORE_FLOW_CONTROL__HPP

#include "IEvents.hpp"
#include "JoystickManager.hpp"
#include "chimera/core/Timer.hpp"

namespace Chimera::IO {

class FlowControl {
  public:
    FlowControl(IEvents* _pGameClientEvents);
    virtual ~FlowControl();
    virtual void open();
    virtual void close();
    void gameLoop();

  private:
    virtual void processaGame();
    void countFrame();

    IEvents* pGameClientEvents;
    JoystickManager joystickManager;
    MouseDevice mouse;
    Timer timerFPS;
};
} // namespace Chimera::IO

#endif