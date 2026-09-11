#pragma once

#include <SDL3/SDL.h>

namespace ce {

    /// @brief Classe de Timer
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20270731
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
        bool stepCount();
        uint32_t deltaCountMS();

        bool isStarted() const { return started; }
        bool isPaused() const { return paused; }
        uint32_t getCountStep() const { return countStep; }
        void setElapsedCount(const uint32_t& val) { elapsedCount = val; }
        double deltaTimeSecounds() { return ((double)deltaCountMS()) / 1000.0F; }

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
