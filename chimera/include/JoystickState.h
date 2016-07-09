#ifndef __JOYSTICK_STATE__H
#define __JOYSTICK_STATE__H

#include <string>
#include <map>

#ifndef WIN32
# include <SDL2/SDL.h>
#else
# include <SDL.h>
#endif

namespace Chimera {

/**
 * Classe de controle do Joystick
 */    
class JoystickState
{
public:
    uint8_t id;
    std::string name;
    SDL_Joystick *pJoystick;
    
   /**
	* Cria novo Joystick valores invalidos
	*/
	JoystickState();

    /**
     * Cria novo Joystick detectado no Manager
     * @param id id sequancial começamdo em 0
     * @param joystick ponteiro do SDL_Joystick
     * @param name nome do joystick detectado pelo SDL_JoystickOpen
     */
    //JoystickState( uint8_t idJoy, SDL_Joystick *joystick, std::string nameJoy);

	//JoystickState(Uint8 id, SDL_Joystick * joystick, std::string name);

	/**
     * Chamado quando um novo evento de joystick é detectado
     * @param event evento a ser tratado
     */
    void TrackEvent ( SDL_Event *event );

    /**
     * Calcula um valor de axix que varia de -32767 a 32768 para -1 a 1
     * @param value valor a transformar
     * @return retorna -1 a 1
     */
    inline static double AxisScale (const Sint16  &value ) {
        return value >= 0 ? ( ( double ) value ) / 32767.0f : ( ( double ) value ) / 32768.0f;
    }

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
    inline double AxisScaled (const Uint8 &axis, const double &low, const double &high, const double &deadzone = 0.0f, const double &deadzone_at_ends = 0.0f ) {
        return low + ( high - low ) * ( Axis ( axis, deadzone, deadzone_at_ends ) + 1.0f ) / 2.0f;
    }
    
    /**
     * Retorna valor do Axis
     * @param axis numero do Axis
     * @param deadzone valor de inicio do deadzone
     * @param deadzone_at_ends valor de fim do deadzone
     * @return valor do axis entre -1 e 1
     */
    double Axis (const Uint8 &axis,const double &deadzone = 0.0f, const double &deadzone_at_ends = 0.0f );

    /**
     * Retorna status do botao selecionado
     * @param button botao a se testar status
     * @return true se precionado
     */
    bool ButtonDown (const Uint8 &button );

    /**
     *  Retorna valor registrado do hat numero Hat
     *  @param hat numero do hat
     *  @return valor retornado
     */
    Uint8 Hat (const Uint8 &hat );
    
    /**
     * Testa se Hat indicado esta apontando para a direcao
     * @param hat numero do Hat
     * @param dir direcao a ser testada
     * @return true se estiver apondando
     */
    inline bool HatDir (const Uint8 &hat, const Uint8 &dir ) {
        return Hat ( hat ) & dir;
    }

    /**
     * Debug do status do Joystick
     * @return string com info do joystick
     */
    std::string GetStatusJoy ( void );
    
private:
    std::map<Uint8, double> Axes;
    std::map<Uint8, bool> ButtonsDown;
    std::map<Uint8, Uint8> Hats;
    std::map<Uint8, int> BallsX;
    std::map<Uint8, int> BallsY;
};
}

#endif

