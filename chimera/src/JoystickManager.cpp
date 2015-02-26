/*
 *  JoystickManager.cpp
 */

#include "JoystickManager.h"

namespace Chimera {

JoystickManager::JoystickManager( void )
{
	Initialized = false;
}


JoystickManager::~JoystickManager( void )
{
	if( Initialized )
	{
		ReleaseJoysticks();
		SDL_JoystickEventState( SDL_DISABLE );
		SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
	}

	Initialized = false;
}


void JoystickManager::Initialize( void )
{
	if( ! Initialized )
		SDL_InitSubSystem( SDL_INIT_JOYSTICK );

	SDL_JoystickEventState( SDL_ENABLE );

	Initialized = true;
}


void JoystickManager::FindJoysticks( void )
{
	if( ! Initialized )
		return;

	for( int i = 0; i < SDL_NumJoysticks(); i ++ )
	{
		if( ! Joysticks[ i ].Joystick )
		{
			Joysticks[ i ].ID = i;
			Joysticks[ i ].Joystick = SDL_JoystickOpen( i );
			if( Joysticks[ i ].Joystick )
			{
				//const char *joystick_name = SDL_JoystickName( i );
	   			const char *joystick_name = SDL_JoystickName(Joysticks[ i ].Joystick );
				Joysticks[ i ].Name = joystick_name ? joystick_name : "Joystick";
			}
		}
	}
}


void JoystickManager::ReleaseJoysticks( void )
{
	if( ! Initialized )
		return;

	for( std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter ++ )
	{
		if( joy_iter->second.Joystick )
			SDL_JoystickClose( joy_iter->second.Joystick );
		joy_iter->second.Joystick = NULL;
		joy_iter->second.Name = "Disconnected";
	}

	Joysticks.clear();
}


bool JoystickManager::TrackEvent(SDL_Event *event)
{
	Uint8 id = 255;
	switch (event->type){
	case SDL_JOYAXISMOTION:
		id = event->jaxis.which;
		break;
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
		id = event->jbutton.which;
		break;
	case SDL_JOYHATMOTION:
		id = event->jhat.which;
		break;
	case SDL_JOYBALLMOTION:
		id = event->jball.which;
		break;
	default:
		return false;
	}

	Joysticks[id].ID = id;
	Joysticks[id].TrackEvent(event);
	return true;

}


double JoystickManager::Axis( int joystick_id, Uint8 axis, double deadzone, double deadzone_at_ends )
{
	// Return the value of a joystick's axis.
	// Note that these values are scaled to the (-1,1) range when tracked.

	std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.find( joystick_id );
	if( joy_iter != Joysticks.end() )
		return joy_iter->second.Axis( axis, deadzone, deadzone_at_ends );

	return 0.;
}


double JoystickManager::AxisScaled( int joystick_id, Uint8 axis, double low, double high, double deadzone, double deadzone_at_ends )
{
	// Return the value of a joystick's axis, scaled to (low,high).

	std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.find( joystick_id );
	if( joy_iter != Joysticks.end() )
		return joy_iter->second.AxisScaled( axis, low, high, deadzone, deadzone_at_ends );

	return low + (high - low) / 2.;
}


bool JoystickManager::ButtonDown( int joystick_id, Uint8 button )
{
	std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.find( joystick_id );
	if( joy_iter != Joysticks.end() )
		return joy_iter->second.ButtonDown( button );

	return false;
}


Uint8 JoystickManager::Hat( int joystick_id, Uint8 hat )
{
	std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.find( joystick_id );
	if( joy_iter != Joysticks.end() )
		return joy_iter->second.Hat( hat );

	return 0;
}


bool JoystickManager::HatDir( int joystick_id, Uint8 hat, Uint8 dir )
{
	// See if the hat switch is in this cardinal direction.
	// This matches straight directions even if the hat is being pushed diagonally.

	std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.find( joystick_id );
	if( joy_iter != Joysticks.end() )
		return joy_iter->second.HatDir( hat, dir );

	return false;
}


std::string JoystickManager::GetStatusManager( void )
{
	// Create a status string for all joysticks.

	std::string return_string;
	char cstr[ 1024 ] = "";

	sprintf_s(cstr, 1024, "Joysticks: %i", Joysticks.size());
	return_string += cstr;

	for( std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter ++ )
		return_string += "\n" + joy_iter->second.GetStatusJoy();

	return return_string;
}
}

