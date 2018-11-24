#ifndef __CHIMERA_JOYSTICK_MANAGER__HPP
#define __CHIMERA_JOYSTICK_MANAGER__HPP

#include <map>
#include <string>

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "JoystickState.h"

namespace Chimera {

/**
 * Enumerador dos codigos dos Axis do Joystico do XBOX360
 */
enum class JOY_AXIX_COD {
    LEFT_X = 0,
    LEFT_Y = 1,
    LEFT_TRIGGER = 2,
    RIGHT_X = 3,
    RIGHT_Y = 4,
    RIGHT_TRIGGER = 5,
};

/**
 * Enumerador dos codigos dos botoes do Joystic XBOX360
 */
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

/**
 * Enumerador dos PAD's do Joystic XBOX360
 */
enum class JOY_PAD_COD {
    UP = 0b00000001,
    RIGHT = 0b00000010,
    DOWN = 0b00000100,
    LEFT = 0b00001000
};

/**
 * Classe gerenciadora de todos os Joysticks identificados
 */
class JoystickManager {
  public:
    JoystickManager(void);
    virtual ~JoystickManager(void);

    /**
     * Inicializa o SDL para uso de Joystic
     */
    void Initialize(void);

    /**
     * Libera Todos os Joysticks detectados
     */
    void ReleaseJoysticks(void);

    /**
     * Realiza um rastreamento para localizar todos os Joystics ativos
     */
    void FindJoysticks(void);

    /**
     * Retorna o Joystic identificado pelo id que comeca em 0
     * @param joystick_id identificado do Joystic
     * @return Retorna o joystic identificado ou null caso ele nao esteja inserido
     */
    JoystickState* getJoystickState(const int& joystick_id);

    /**
     * Executa leitura dos controle
     * @return true se há modificacao nos Joysticks
     */
    bool TrackEvent(SDL_Event* event);

    /**
     * Retorna uma string com a lista de Joysticks identificado
     * @return nome e dados do joystick
     */
    std::string GetStatusManager(void);

    /**
     * Debug dos status dos Joysticks inseridos
     */
    void DebugDadosJoystick();

  private:
    std::map<Uint8, JoystickState> Joysticks;
    bool Initialized;
};
} // namespace Chimera
#endif
