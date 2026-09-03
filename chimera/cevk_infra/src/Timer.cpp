#include "cevk_infra/Timer.hpp"

namespace ce {
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

    uint32_t Timer::restart() {
        uint32_t elapsedTicks = ticks();
        start();
        return elapsedTicks;
    }

    uint32_t Timer::ticks() const {
        if (started) {
            if (!paused) {
                return SDL_GetTicks() - startTicks;
            }
            return pausedTicks;
        }
        return 0;
    }

    bool Timer::stepCount() {

        uint32_t temp = ticks();
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

    uint32_t Timer::deltaCountMS() {
        uint32_t current = SDL_GetTicks();
        uint32_t val = current - lastTicks;
        lastTicks = current;
        return val;
    }
} // namespace ce
