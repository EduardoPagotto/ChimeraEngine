#ifndef TIMER_H_
#define TIMER_H_

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

namespace Chimera {

/**
 * Class Timer
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Timer {
public:
    Timer();
    virtual ~Timer();

    void start();
    void stop();
    void pause();
    void resume();

    Uint32 restart();
    Uint32 ticks() const;
    bool stepCount();

    inline bool isStarted() const {
        return started;
    }
    inline bool isPaused() const {
        return paused;
    }
    inline Uint32 getCountStep() const {
        return countStep;
    }
    inline void setElapsedCount (const Uint32 &val ) {
        elapsedCount = val;
    }

    
private:
    bool started;
    bool paused;
    Uint32 startTicks;
    Uint32 pausedTicks;
    Uint32 step;
    Uint32 countStep;
    Uint32 elapsedCount;
};

} /* namespace Chimera */
#endif /* TIMER_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
