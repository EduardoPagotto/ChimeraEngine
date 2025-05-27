#pragma once
#include "ServiceLocator.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace ce {

    /// @brief Mouse Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20250527
    class Mouse : public IService {

      private:
        std::map<uint8_t, bool> buttonState;
        glm::ivec2 pos{glm::ivec2(0)}, rel{glm::ivec2(0)}, wheel{glm::ivec2(0)};
        uint32_t flag1{0}, flag2{0};

      public:
        Mouse() noexcept = default;

        virtual ~Mouse() noexcept override = default;

        // IService base
        std::type_index getTypeIndex() const override { return std::type_index(typeid(Mouse)); }

        const bool getButtonState(const uint8_t& indice) noexcept {

            if (this->buttonState.contains(indice))
                return buttonState[indice];

            return false;
        }

        const glm::ivec2 getMove() const noexcept { return pos; }

        const glm::ivec2 getMoveRel() noexcept {
            if (flag1 != flag2) {
                flag1 = flag2;
                return rel;
            }
            return glm::ivec2(0);
        }

        const bool getEvent(const SDL_Event& event) noexcept {
            switch (event.type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    this->updateBt(event.button);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    this->updateMv(event.motion);
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    this->updateWl(event.wheel);
                    break;
            }
            return false;
        }

        void updateBt(const SDL_MouseButtonEvent& bt) noexcept { this->buttonState[bt.button] = bt.down; }

        void updateMv(const SDL_MouseMotionEvent& mv) noexcept {
            this->pos = glm::ivec2(mv.x, mv.y);
            this->rel = glm::ivec2(mv.xrel, mv.yrel);
            flag1++;
        }

        void updateWl(const SDL_MouseWheelEvent& mwe) noexcept { this->wheel = glm::ivec2(mwe.x, mwe.y); }
    };
} // namespace ce
