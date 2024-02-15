#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace Chimera {
class Mouse {
  private:
    std::map<uint8_t, uint8_t> m_buttonState;
    glm::ivec2 m_pos{0}, m_rel{0}, m_wheel{0};
    uint32_t m_flag1{0}, m_flag2{0};

  public:
    Mouse() = default;
    virtual ~Mouse() = default;

    inline uint8_t const getButtonState(const uint8_t& indice) {

        if (this->m_buttonState.contains(indice))
            return m_buttonState[indice];

        return SDL_RELEASED;
    }

    inline const glm::ivec2 getMove() const { return m_pos; }

    inline const glm::ivec2 getMoveRel() {
        if (m_flag1 != m_flag2) {
            m_flag1 = m_flag2;
            return m_rel;
        }
        return glm::ivec2(0);
    }

    inline void updateBt(const SDL_MouseButtonEvent& bt) { this->m_buttonState[bt.button] = bt.state; }

    inline void updateMv(const SDL_MouseMotionEvent& mv) {
        this->m_pos = glm::ivec2(mv.x, mv.y);
        this->m_rel = glm::ivec2(mv.xrel, mv.yrel);
        m_flag1++;
    }

    inline void updateWl(const SDL_MouseWheelEvent& mwe) { this->m_wheel = glm::ivec2(mwe.x, mwe.y); }

    inline bool getEvent(const SDL_Event& event) {
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
};
} // namespace Chimera
