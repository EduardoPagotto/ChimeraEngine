#pragma once

#include <SDL2/SDL.h>

namespace ce {

/**
 * Class Timer
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Timer {
  public:
    Timer() = default;

    virtual ~Timer() = default;

    void start() {
        started = true;
        paused = false;
        startTicks = SDL_GetTicks();
        lastTicks = startTicks;
    }

    void stop() {
        started = false;
        paused = false;
    }

    void pause() {
        if (started && !paused) {
            paused = true;
            pausedTicks = SDL_GetTicks() - startTicks;
        }
    }

    void resume() {
        if (paused) {
            paused = false;
            startTicks = SDL_GetTicks() - pausedTicks;
            lastTicks = startTicks;
            pausedTicks = 0;
        }
    }

    uint32_t restart() {
        uint32_t elapsedTicks = ticks();
        start();
        return elapsedTicks;
    }

    const uint32_t ticks() const {
        if (started) {
            if (!paused) {
                return SDL_GetTicks() - startTicks;
            } else {
                return pausedTicks;
            }
        }
        return 0;
    }

    bool stepCount() {

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

    inline bool isStarted() const { return started; }
    inline bool isPaused() const { return paused; }
    inline uint32_t getCountStep() const { return countStep; }
    inline void setElapsedCount(const uint32_t& val) { elapsedCount = val; }

    inline double deltaTimeSecounds() { return ((double)deltaCountMS()) / 1000.0f; }

    inline uint32_t deltaCountMS() {
        uint32_t current = SDL_GetTicks();
        uint32_t val = current - lastTicks;
        lastTicks = current;
        return val;
    }

  private:
    bool started = false;
    bool paused = false;
    uint32_t startTicks = 0;
    uint32_t lastTicks = 0;
    uint32_t pausedTicks = 0;
    uint32_t step = 0;
    uint32_t countStep = 0;
    uint32_t elapsedCount = 0;
};
} // namespace ce
