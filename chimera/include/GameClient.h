#ifndef __CHIMERA_GAME_CLIENT__HPP
#define __CHIMERA_GAME_CLIENT__HPP

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "DataMsg.h"

namespace Chimera {
void sendMessage(KindOp _kindOf, void* _paramA, void* _paramB);
}

#endif
