#pragma once
#include "ServiceLocator.hpp"
#include <SDL2/SDL.h>

namespace ce {

/// @brief Canvas Interface
/// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
/// @since 20130925
/// @date 20250126
class ICanva : public IService {
  public:
    virtual ~ICanva() = default;
    virtual void before() = 0;
    virtual void after() = 0;
    virtual void toggleFullScreen() = 0;
    virtual void reshape(int _width, int _height) = 0;
    virtual uint32_t* getPixels() = 0;
    virtual const int getWidth() const = 0;
    virtual const int getHeight() const = 0;
};

struct CanvasComponent {
    std::shared_ptr<ICanva> canvas = nullptr;
    CanvasComponent() = default;
};

class IMouse : public IService {
  public:
    virtual ~IMouse() = default;
    virtual const uint8_t getButtonState(const uint8_t& indice) noexcept = 0;
    virtual const SDL_Point getMove() const noexcept = 0;
    virtual const SDL_Point getMoveRel() noexcept = 0;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
    virtual void updateBt(const SDL_MouseButtonEvent& bt) noexcept = 0;
    virtual void updateMv(const SDL_MouseMotionEvent& mv) noexcept = 0;
    virtual void updateWl(const SDL_MouseWheelEvent& mwe) noexcept = 0;
};

class IKeyboard : public IService {
  public:
    virtual ~IKeyboard() = default;
    virtual void setDown(const SDL_KeyboardEvent& event) noexcept = 0;
    virtual void setUp(const SDL_KeyboardEvent& event) noexcept = 0;
    virtual const bool isPressed(const SDL_Keycode& key) noexcept = 0;
    virtual const bool isModPressed(const SDL_Keymod& keyMod) const noexcept = 0;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
};

class IJoystick : public IService {
  public:
    virtual ~IJoystick() = default;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
    virtual SDL_Joystick* get(const SDL_JoystickID& joystick_id) noexcept = 0;
};

class IGameController : public IService {
  public:
    virtual ~IGameController() = default;
    virtual const bool getEvent(const SDL_Event& event) noexcept = 0;
    virtual SDL_GameController* get(const SDL_JoystickID& joystick_id) noexcept = 0;
};

} // namespace ce
