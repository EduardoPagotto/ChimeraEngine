#ifndef __CHIMERA_FLOW_CONTROL__HPP
#define __CHIMERA_FLOW_CONTROL__HPP

#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/core/Timer.hpp"
#include <spdlog/spdlog.h>

namespace Chimera {

class FlowControl {
  public:
    FlowControl(IGameClientEvents* _pGameClientEvents);
    virtual ~FlowControl();
    virtual void open();
    virtual void close();
    void gameLoop();

  private:
    virtual void processaGame();
    void countFrame();

    IGameClientEvents* pGameClientEvents;
    JoystickManager joystickManager;
    Timer timerFPS;

    std::shared_ptr<spdlog::logger> log;
};
} // namespace Chimera

#endif