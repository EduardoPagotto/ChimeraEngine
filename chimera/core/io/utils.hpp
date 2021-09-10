#ifndef __CHIMERA_UTILS__HPP
#define __CHIMERA_UTILS__HPP

#include <SDL2/SDL.h>

namespace Chimera::IO {

//#define TOGGLE_FULL_SCREEN 0x0001
const int32_t TOGGLE_FULL_SCREEN = 0x0001;

/**
 * Enum KindOp
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
void inline eventsSend(const int32_t& user_event, void* _paramA, void* _paramB) {
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_USEREVENT;
    event.user.code = user_event;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    SDL_PushEvent(&event);
}

// GLuint shadeLoadProg(const char* programName, const char* vertex_file_path, const char* fragment_file_path);

} // namespace Chimera::IO

#endif
