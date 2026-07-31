#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace ce {

    /// @brief Mouse Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260731
    class Mouse {

      public:
        Mouse() noexcept = default;
        virtual ~Mouse() noexcept = default;

        const bool getButtonState(const uint8_t& indice) noexcept;
        const glm::ivec2 getMove() const noexcept { return pos; }
        const glm::ivec2 getMoveRel() noexcept;

        const bool getEvent(const SDL_Event& event) noexcept;

        void updateBt(const SDL_MouseButtonEvent& bt) noexcept { this->buttonState[bt.button] = bt.down; }
        void updateMv(const SDL_MouseMotionEvent& mv) noexcept;
        void updateWl(const SDL_MouseWheelEvent& mwe) noexcept { this->wheel = glm::ivec2(mwe.x, mwe.y); }

      private:
        std::map<uint8_t, bool> buttonState;
        glm::ivec2 pos{glm::ivec2(0)}, rel{glm::ivec2(0)}, wheel{glm::ivec2(0)};
        uint32_t flag1{0}, flag2{0};
    };
} // namespace ce
