/*
 *  JoystickState.h
 */

#ifndef __JOYSTICK_STATE__H
#define __JOYSTICK_STATE__H

//class JoystickState;

//#include "platforms.h"  // Include platform-specific headers.

#include <string>
#include <map>

#ifndef WIN32
# include <SDL2/SDL.h>
#else
# include <SDL.h>
#endif

namespace Chimera {

class JoystickState
{
public:
	uint8_t ID;
	SDL_Joystick *Joystick;
	std::string Name;
	std::map<Uint8, double> Axes;
	std::map<Uint8, bool> ButtonsDown;
	std::map<Uint8, Uint8> Hats;
	std::map<Uint8, int> BallsX;
	std::map<Uint8, int> BallsY;

	/**
     * Cria novo Joystick detectado no Manager
	 * @param id id sequancial começamdo em 0
	 * @param joystick ponteiro do SDL_Joystick
	 * @param name nome do joystick detectado pelo SDL_JoystickOpen
     */
	JoystickState( uint8_t id = 255, SDL_Joystick *joystick = NULL, std::string name = "Joystick" );

	/**
     * Chamado quando um novo evento de joystick é detectado
	 * @param event evento a ser tratado
     */
	void TrackEvent( SDL_Event *event );

	/**
	 * Transforma um valor de -32767 a 32768 para -1 a 1
	 * @param value valor a transformar
	 * @return retorna -1 a 1
     */
	static double AxisScale( Sint16 value );

	/**
     * Retorna valor do Axis
	 * @param axis numero do Axis
     * @param deadzone valor de inicio do deadzone
	 * @param deadzone_at_ends valor de fim do deadzone
     * @return valor do axis entre -1 e 1
     */
	double Axis( Uint8 axis, double deadzone = 0., double deadzone_at_ends = 0. );

	/**
     * Retorna valor do Axis,  parametriando inicioe fim<p>
	 * Pode ser usado para inverter o eixo
	 * @param axis numero do Axis
     * @param low valor low
	 * @param high valor Hight
	 * @param deadzone valor de inicio do deadzone
	 * @param deadzone_at_ends valor de fim do deadzone
	 * @return valor do axis entre -1 e 1
     */
	double AxisScaled( Uint8 axis, double low, double high, double deadzone = 0., double deadzone_at_ends = 0. );

	/**
     * Retorna status do botao selecionado
	 * @param button botao a se testar status
     * @return true se precionado
     */
	bool ButtonDown( Uint8 button );

	Uint8 Hat( Uint8 hat );
	bool HatDir( Uint8 hat, Uint8 dir );

	std::string GetStatusJoy(void);
};

}

#endif

