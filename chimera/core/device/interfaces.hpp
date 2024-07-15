#pragma once
#include "chimera/core/ServiceLocator.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Chimera {

class IMouse : public IService {
  public:
    virtual ~IMouse() noexcept = default;
    virtual const uint8_t getButtonState(const uint8_t& indice) noexcept = 0;
    virtual const glm::ivec2 getMove() const noexcept = 0;
    virtual const glm::ivec2 getMoveRel() noexcept = 0;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
    virtual void updateBt(const SDL_MouseButtonEvent& bt) noexcept = 0;
    virtual void updateMv(const SDL_MouseMotionEvent& mv) noexcept = 0;
    virtual void updateWl(const SDL_MouseWheelEvent& mwe) noexcept = 0;
};

class IKeyboard : public IService {
  public:
    virtual ~IKeyboard() noexcept = default;
    virtual void setDown(const SDL_KeyboardEvent& event) noexcept = 0;
    virtual void setUp(const SDL_KeyboardEvent& event) noexcept = 0;
    virtual const bool isPressed(const SDL_Keycode& key) noexcept = 0;
    virtual const bool isModPressed(const SDL_Keymod& keyMod) const noexcept = 0;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
};

class IJoystick : public IService {
  public:
    virtual ~IJoystick() noexcept = default;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
    virtual SDL_Joystick* get(const SDL_JoystickID& joystick_id) noexcept = 0;
};

class IGameController : public IService {
  public:
    virtual ~IGameController() noexcept = default;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
    virtual SDL_GameController* get(const SDL_JoystickID& joystick_id) noexcept = 0;
};

} // namespace Chimera