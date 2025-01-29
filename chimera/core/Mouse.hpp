#pragma once
#include "interfaces.hpp"
#include <map>

namespace ce {
class Mouse : public ServiceBase<IMouse> {
  private:
    std::map<uint8_t, uint8_t> buttonState;
    SDL_Point pos{0, 0}, rel{0, 0}, wheel{0, 0};
    uint32_t flag1{0}, flag2{0};

  public:
    Mouse() noexcept = default;
    virtual ~Mouse() = default;

    const uint8_t getButtonState(const uint8_t& indice) noexcept override {

        if (this->buttonState.contains(indice))
            return buttonState[indice];

        return SDL_RELEASED;
    }

    const SDL_Point getMove() const noexcept override { return pos; }

    const SDL_Point getMoveRel() noexcept override {
        if (flag1 != flag2) {
            flag1 = flag2;
            return rel;
        }
        return SDL_Point{0, 0};
    }

    const bool getEvent(const SDL_Event& event) noexcept override {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                this->updateBt(event.button);
                break;
            case SDL_MOUSEMOTION:
                this->updateMv(event.motion);
                break;
            case SDL_MOUSEWHEEL:
                this->updateWl(event.wheel);
                break;
        }
        return false;
    }

    void updateBt(const SDL_MouseButtonEvent& bt) noexcept override { this->buttonState[bt.button] = bt.state; }

    void updateMv(const SDL_MouseMotionEvent& mv) noexcept override {
        this->pos = SDL_Point{mv.x, mv.y};
        this->rel = SDL_Point{mv.xrel, mv.yrel};
        flag1++;
    }

    void updateWl(const SDL_MouseWheelEvent& mwe) noexcept override { this->wheel = SDL_Point{mwe.x, mwe.y}; }
};
} // namespace ce
