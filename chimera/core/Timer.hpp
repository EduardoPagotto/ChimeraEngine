#pragma once

#include <SDL2/SDL.h>

namespace ce {

/// @brief Class Timer
/// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
/// @since 20130925
/// @date  20250125
class Timer {
    bool started{false}, paused{false};
    uint32_t startTicks{0}, lastTicks{0}, pausedTicks{0}, step{0}, countStep{0}, elapsedCount{0};

  public:
    Timer() = default;

    virtual ~Timer() = default;

    void start() {
        this->started = true;
        this->paused = false;
        this->startTicks = SDL_GetTicks();
        this->lastTicks = this->startTicks;
    }

    void stop() {
        this->started = false;
        this->paused = false;
    }

    void pause() {
        if (this->started && !this->paused) {
            this->paused = true;
            this->pausedTicks = SDL_GetTicks() - this->startTicks;
        }
    }

    void resume() {
        if (this->paused) {
            this->paused = false;
            this->startTicks = SDL_GetTicks() - this->pausedTicks;
            this->lastTicks = this->startTicks;
            this->pausedTicks = 0;
        }
    }

    inline const uint32_t restart() {
        const uint32_t elapsedTicks = ticks();
        start();
        return elapsedTicks;
    }

    const uint32_t ticks() const {
        if (this->started) {
            if (!this->paused) {
                return SDL_GetTicks() - this->startTicks;
            } else {
                return this->pausedTicks;
            }
        }
        return 0;
    }

    const bool stepCount() {

        if (this->ticks() < elapsedCount) {
            this->step++;
        } else {
            this->countStep = this->step;
            this->step = 0;
            this->start();
            return true;
        }

        return false;
    }

    inline bool isStarted() const { return this->started; }
    inline bool isPaused() const { return this->paused; }
    inline uint32_t getCountStep() const { return this->countStep; }
    inline void setElapsedCount(const uint32_t& val) { this->elapsedCount = val; }

    inline double deltaTimeSecounds() { return ((double)this->deltaCountMS()) / 1000.0f; }

    inline const uint32_t deltaCountMS() {
        const uint32_t current = SDL_GetTicks();
        const uint32_t val = current - this->lastTicks;
        this->lastTicks = current;
        return val;
    }
};
} // namespace ce
