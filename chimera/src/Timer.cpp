#include "Timer.h"

namespace Chimera {

Timer::Timer() :
    startTicks ( 0 ), pausedTicks ( 0 ), started ( false ), paused ( false ),step ( 0 ),countStep ( 0 ),elapsedCount ( 0 ) {

}

Timer::~Timer() {

}

void Timer::start() {
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop() {
    started = false;
    paused = false;
}

void Timer::pause() {
    if ( started && !paused ) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::resume() {
    if ( paused ) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

int Timer::restart() {
    int elapsedTicks = ticks();
    start();
    return elapsedTicks;
}

int Timer::ticks() const {
    if ( started ) {
        if ( paused ) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

bool Timer::stepCount() {

    int temp = ticks();
    if ( temp < elapsedCount ) {
        step++;
    } else {
        countStep = step;
        step = 0;
        start();
        return true;
    }

    return false;

}

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
