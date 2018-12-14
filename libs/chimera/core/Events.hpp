#ifndef __CHIMERA_EVENTS__HPP
#define __CHIMERA_EVENTS__HPP

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

//#include "chimera/core/DataMsg.hpp"

namespace Chimera {

/**
 * Enum KindOp
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class KindOp { IS_ALLOW_COLLIDE, START_COLLIDE, ON_COLLIDE, OFF_COLLIDE, VIDEO_TOGGLE_FULL_SCREEN };

void eventsSend(KindOp _kindOf, void* _paramA, void* _paramB);
} // namespace Chimera

#endif
