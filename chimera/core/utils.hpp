#ifndef __CHIMERA_UTILS__HPP
#define __CHIMERA_UTILS__HPP

#include <SDL2/SDL.h>

namespace Chimera {

/**
 * Enum KindOp
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class KindOp { IS_ALLOW_COLLIDE, START_COLLIDE, ON_COLLIDE, OFF_COLLIDE, VIDEO_TOGGLE_FULL_SCREEN };

void inline eventsSend(KindOp _kindOf, void* _paramA, void* _paramB) {
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_USEREVENT;
    event.user.code = (int)_kindOf;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    SDL_PushEvent(&event);
}

// GLuint shadeLoadProg(const char* programName, const char* vertex_file_path, const char* fragment_file_path);

} // namespace Chimera

#endif
