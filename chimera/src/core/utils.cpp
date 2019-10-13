#include "chimera/core/utils.hpp"
#include "chimera/core/Exception.hpp"

#include <SDL2/SDL.h>

#include <fstream>
#include <string>
#include <vector>

namespace Chimera {

// void eventsSend(KindOp _kindOf, void* _paramA, void* _paramB) {
//     SDL_Event event;
//     SDL_zero(event);
//     event.type = SDL_USEREVENT;
//     event.user.code = (int)_kindOf;
//     event.user.data1 = _paramA;
//     event.user.data2 = _paramB;
//     SDL_PushEvent(&event);
// }

} // namespace Chimera