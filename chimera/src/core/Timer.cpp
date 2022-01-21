#include "chimera/core/Timer.hpp"

namespace Chimera {

Timer::Timer() : startTicks(0), pausedTicks(0), started(false), paused(false), step(0), countStep(0), elapsedCount(0), lastTicks(0) {}

Timer::~Timer() {}

void Timer::start() {
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
    lastTicks = startTicks;
}

void Timer::stop() {
    started = false;
    paused = false;
}

void Timer::pause() {
    if (started && !paused) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::resume() {
    if (paused) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        lastTicks = startTicks;
        pausedTicks = 0;
    }
}

Uint32 Timer::restart() {
    Uint32 elapsedTicks = ticks();
    start();
    return elapsedTicks;
}

Uint32 Timer::ticks() const {
    if (started) {
        if (!paused) {
            return SDL_GetTicks() - startTicks;
        } else {
            return pausedTicks;
        }
    }
    return 0;
}

bool Timer::stepCount() {

    Uint32 temp = ticks();
    if (temp < elapsedCount) {
        step++;
    } else {
        countStep = step;
        step = 0;
        start();
        return true;
    }

    return false;
}
} // namespace Chimera
