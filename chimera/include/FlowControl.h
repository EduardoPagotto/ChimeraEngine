#ifndef __FLOW_CONTROL_H
#define __FLOW_CONTROL_H

#include "IGameClientEvents.h"
# include "Video.h"
# include "Timer.h"

namespace Chimera
{

class FlowControl
{
public:
    FlowControl ( Video *_pVideo, IGameClientEvents *_pGameClientEvents );
    virtual ~FlowControl();
    virtual void open();
    virtual void close();
    void gameLoop();

private:
    virtual void processaGame();
    void countFrame();

    Video *pVideo;
    IGameClientEvents *pGameClientEvents;
    JoystickManager joystickManager;
    Timer timerFPS;
};
}

# endif