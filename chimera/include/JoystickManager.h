#ifndef __JOYSTICK_MANAGER__H
# define __JOYSTICK_MANAGER__H

#include <string>
#include <map>

#ifndef WIN32
# include <SDL2/SDL.h>
#else
# include <SDL.h>
#endif

#include "JoystickState.h"

namespace Chimera {
namespace Device {
    
enum class JOY_AXIX_COD {
	LEFT_X = 0,
	LEFT_Y = 1,
	LEFT_TRIGGER = 2,
	RIGHT_X = 3,
	RIGHT_Y = 4,
	RIGHT_TRIGGER = 5,
};

enum class JOY_BUTTON_COD {
	A,
	B,
	X,
	Y,
	LEFT_BUMPER,
	RIGHT_BUMPER,
	BACK,
	START,
	LEFT_STICK_CLICK,
	RIGHT_STICK_CLICK,
};

class JoystickManager
{
public:
    std::map<Uint8, JoystickState> Joysticks;
    bool Initialized;

    JoystickManager ( void );
    virtual ~JoystickManager ( void );

    void Initialize ( void );
    void FindJoysticks ( void );
    void ReleaseJoysticks ( void );

    JoystickState* getJoystickState ( int joystick_id );

    bool TrackEvent ( SDL_Event *event );

    //double Axis( int joystick_id, Uint8 axis, double deadzone = 0., double deadzone_at_ends = 0. );
    //double AxisScaled( int joystick_id, Uint8 axis, double low, double high, double deadzone = 0., double deadzone_at_ends = 0. );
    //bool ButtonDown( int joystick_id, Uint8 button );
    //Uint8 Hat( int joystick_id, Uint8 hat );
    //bool HatDir( int joystick_id, Uint8 hat, Uint8 dir );

    std::string GetStatusManager ( void );

    void DebugDadosJoystick();
};
}
}
# endif

