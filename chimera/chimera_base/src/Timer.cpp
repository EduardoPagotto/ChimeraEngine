#include "chimera_base/Timer.hpp"

namespace ce {
    void Timer::start() {
        started_ = true;
        paused_ = false;
        start_ticks_ = SDL_GetTicks();
        last_ticks_ = start_ticks_;
    }

    void Timer::stop() {
        started_ = false;
        paused_ = false;
    }

    void Timer::pause() {
        if (started_ && !paused_) {
            paused_ = true;
            paused_ticks_ = SDL_GetTicks() - start_ticks_;
        }
    }

    void Timer::resume() {
        if (paused_) {
            paused_ = false;
            start_ticks_ = SDL_GetTicks() - paused_ticks_;
            last_ticks_ = start_ticks_;
            paused_ticks_ = 0;
        }
    }

    uint32_t Timer::restart() {
        const uint32_t elapsed_ticks = ticks();
        start();
        return elapsed_ticks;
    }

    uint32_t Timer::ticks() const {
        if (started_) {
            if (!paused_) {
                return SDL_GetTicks() - start_ticks_;
            }
            return paused_ticks_;
        }
        return 0;
    }

    bool Timer::step_count() {

        const uint32_t temp = ticks();
        if (temp < elapsed_count_) {
            step_++;
        } else {
            count_step_ = step_;
            step_ = 0;
            start();
            return true;
        }

        return false;
    }

    uint32_t Timer::delta_count_ms() {
        const uint32_t current = SDL_GetTicks();
        const uint32_t val = current - last_ticks_;
        last_ticks_ = current;
        return val;
    }
} // namespace ce
