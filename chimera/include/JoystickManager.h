/*
 *  JoystickManager.h
 */

#ifndef __JOYSTICK_MANAGER__H
# define __JOYSTICK_MANAGER__H

//class JoystickManager;

//#include "platforms.h"  // Include platform-specific headers.

#include <string>
#include <map>

#ifndef WIN32
# include <SDL2/SDL.h>
#else
# include <SDL.h>
#endif

#include "JoystickState.h"

namespace Chimera {

class JoystickManager
{
public:
	std::map<Uint8, JoystickState> Joysticks;
	bool Initialized;

	JoystickManager( void );
	virtual ~JoystickManager( void );

	void Initialize( void );
	void FindJoysticks( void );
	void ReleaseJoysticks( void );

	bool TrackEvent( SDL_Event *event );
	double Axis( int joystick_id, Uint8 axis, double deadzone = 0., double deadzone_at_ends = 0. );
	double AxisScaled( int joystick_id, Uint8 axis, double low, double high, double deadzone = 0., double deadzone_at_ends = 0. );
	bool ButtonDown( int joystick_id, Uint8 button );
	Uint8 Hat( int joystick_id, Uint8 hat );
	bool HatDir( int joystick_id, Uint8 hat, Uint8 dir );

	std::string GetStatusManager( void );

	void DebugDadosJoystick();
};
}

# endif

