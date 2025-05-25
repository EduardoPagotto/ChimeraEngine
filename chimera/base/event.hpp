#pragma once
#include <SDL2/SDL.h>

namespace ce {

    /// @brief EventsME
    enum class EventCE : int32_t {
        TOGGLE_FULL_SCREEN = 0x0001, // FullScreem
        FLOW_PAUSE = 0x0002,         // set to pause game
        FLOW_RESUME = 0x0003,        // set to resume game
        FLOW_STOP = 0x0004,          // run finalize game
        NEW_FPS = 0x0005,            // OFF_COLLIDE
        COLLIDE_IS_ALLOW = 0x1000,   // IS_ALLOW_COLLIDE
        COLLIDE_START = 0x1001,      // START_COLLIDE
        COLLIDE_ON = 0x1002,         // ON_COLLIDE
        COLLIDE_OFF = 0x1003,        // OFF_COLLIDE
    };

    /// @brief Chimera events pulling
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @param user_event Event kind
    /// @param paramA pointerA
    /// @param paramB PointerB
    /// @return True if ok
    inline void sendChimeraEvent(const EventCE& user_event, void* _paramA, void* _paramB) {
        SDL_Event event;
        SDL_zero(event);
        event.type = SDL_USEREVENT;
        event.user.code = static_cast<Sint32>(user_event);
        event.user.data1 = _paramA;
        event.user.data2 = _paramB;
        if (SDL_PushEvent(&event) == -1) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical PushEvent fail: %s", SDL_GetError());
        }
    }

} // namespace ce
