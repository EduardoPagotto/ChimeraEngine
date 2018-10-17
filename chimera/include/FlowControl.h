#ifndef __FLOW_CONTROL_H
#define __FLOW_CONTROL_H

#include "IGameClientEvents.h"
# include "Timer.h"
#include <spdlog/spdlog.h>

namespace Chimera
{

class FlowControl
{
public:
    FlowControl ( IGameClientEvents *_pGameClientEvents );
    virtual ~FlowControl();
    virtual void open();
    virtual void close();
    void gameLoop();

private:
    virtual void processaGame();
    void countFrame();

    IGameClientEvents *pGameClientEvents;
    JoystickManager joystickManager;
    Timer timerFPS;
    
    std::shared_ptr<spdlog::logger> log;
};
}

# endif