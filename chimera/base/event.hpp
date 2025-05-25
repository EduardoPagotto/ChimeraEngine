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

    // /// @brief Chimera events pulling
    // /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    // /// @since 20130925
    // /// @param user_event Event kind
    // /// @param paramA pointerA
    // /// @param paramB PointerB
    // /// @return True if ok
    // inline bool sendChimeraEvent(const EventCE& user_event, void* paramA, void* paramB) {

    //     SDL_Event event;
    //     SDL_zero(event);
    //     event.type = 0; // SDL_EVENT_USER;
    //     event.user.code = static_cast<Sint32>(user_event);
    //     event.user.data1 = paramA;
    //     event.user.data2 = paramB;
    //     if (SDL_PushEvent(&event)) {
    //         return true;
    //     }

    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical PushEvent fail: %s", SDL_GetError());
    //     return false;
    // }

    const int32_t EVENT_TOGGLE_FULL_SCREEN = 0x0001; // FullScreem
    const int32_t EVENT_FLOW_PAUSE = 0x0002;         // set to pause game
    const int32_t EVENT_FLOW_RESUME = 0x0003;        // set to resume game
    const int32_t EVENT_FLOW_STOP = 0x0004;          // run finalize game
    const int32_t EVENT_NEW_FPS = 0x0005;            // OFF_COLLIDE
    const int32_t EVENT_COLLIDE_IS_ALLOW = 0x1000;   // IS_ALLOW_COLLIDE
    const int32_t EVENT_COLLIDE_START = 0x1001;      // START_COLLIDE
    const int32_t EVENT_COLLIDE_ON = 0x1002;         // ON_COLLIDE
    const int32_t EVENT_COLLIDE_OFF = 0x1003;        // OFF_COLLIDE
    /**
     * Enum KindOp
     *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
     *  @since 20130925
     */
    // void utilSendEvent(const int32_t& user_event, void* _paramA, void* _paramB);

    inline void utilSendEvent(const EventCE& user_event, void* _paramA, void* _paramB) {
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
