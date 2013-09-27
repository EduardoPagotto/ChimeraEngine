#ifndef TIMER_H_
#define TIMER_H_

#include <SDL2/SDL.h>

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

    int restart();
    int ticks() const;
    bool stepCount();

    inline bool isStarted() const {
        return started;
    }
    inline bool isPaused() const {
        return paused;
    }
    inline int getCountStep() const {
        return countStep;
    }
    inline void setElapsedCount ( int val ) {
        elapsedCount = val;
    }

private:
    int startTicks, pausedTicks, elapsedCount, countStep, step;
    bool started, paused;

};

} /* namespace Chimera */
#endif /* TIMER_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
