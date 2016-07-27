 #ifndef __GAME_CLIENT_H
 #define __GAME_CLIENT_H
 
#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "DataMsg.h"

 namespace Chimera
 {
	 void sendMessage(KindOp _kindOf, void *_paramA, void *_paramB);
 }
 
 #endif
 // kate: indent-mode cstyle; indent-width 4; replace-tabs on;
