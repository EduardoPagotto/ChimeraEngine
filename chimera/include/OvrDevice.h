#ifndef OVR_DEVICE_H_
#define OVR_DEVICE_H_

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include <string>

#include "OVR.h"

namespace Chimera {

class OvrDevice {
public:
	OvrDevice(std::string nomeTela);
	virtual ~OvrDevice();

private:
	void initOVRSubSys();

	ovrHmd hmd;

	std::string nomeTela;
	SDL_Rect rectangle;
	SDL_Window *window;
	SDL_GLContext context;
};


}

#endif

