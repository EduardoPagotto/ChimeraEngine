#pragma once

#include <SDL3/SDL.h>

namespace ce {

    /// @brief Classe de Timer
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20261004
    class Timer {
      public:
        Timer() = default;
        virtual ~Timer() = default;

        void start();
        void stop();
        void pause();
        void resume();
        uint32_t restart();

        uint32_t ticks() const;
        bool step_count();
        uint32_t delta_count_ms();

        bool is_started() const { return started_; }
        bool is_paused() const { return paused_; }
        uint32_t get_count_step() const { return count_step_; }
        void set_elapsed_count(const uint32_t& val) { elapsed_count_ = val; }
        double delta_time_secounds() { return ((double)delta_count_ms()) / 1000.0F; }

      private:
        bool started_ = false;
        bool paused_ = false;
        uint32_t start_ticks_ = 0;
        uint32_t last_ticks_ = 0;
        uint32_t paused_ticks_ = 0;
        uint32_t step_ = 0;
        uint32_t count_step_ = 0;
        uint32_t elapsed_count_ = 0;
    };
} // namespace ce
