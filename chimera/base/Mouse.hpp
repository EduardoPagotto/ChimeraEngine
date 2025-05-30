#pragma once
#include "interfaces.hpp"
#include <map>

namespace ce {

    class Mouse : public ServiceBase<IMouse> {

      private:
        std::map<uint8_t, bool> buttonState;
        glm::ivec2 pos{glm::ivec2(0)}, rel{glm::ivec2(0)}, wheel{glm::ivec2(0)};
        uint32_t flag1{0}, flag2{0};

      public:
        Mouse() noexcept = default;
        virtual ~Mouse() noexcept override = default;

        const bool getButtonState(const uint8_t& indice) noexcept override {

            if (this->buttonState.contains(indice))
                return buttonState[indice];

            return false;
        }

        const glm::ivec2 getMove() const noexcept override { return pos; }

        const glm::ivec2 getMoveRel() noexcept override {
            if (flag1 != flag2) {
                flag1 = flag2;
                return rel;
            }
            return glm::ivec2(0);
        }

        const bool getEvent(const SDL_Event& event) noexcept override {
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

        void updateBt(const SDL_MouseButtonEvent& bt) noexcept override { this->buttonState[bt.button] = bt.down; }

        void updateMv(const SDL_MouseMotionEvent& mv) noexcept override {
            this->pos = glm::ivec2(mv.x, mv.y);
            this->rel = glm::ivec2(mv.xrel, mv.yrel);
            flag1++;
        }

        void updateWl(const SDL_MouseWheelEvent& mwe) noexcept override { this->wheel = glm::ivec2(mwe.x, mwe.y); }
    };
} // namespace ce
