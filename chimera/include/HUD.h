#ifndef HUD_H_
#define HUD_H_

#include <GL/gl.h>	
#include <GL/glu.h>
#include <vector>

#include <SDL2/SDL.h>

#include "Font.h"

namespace Chimera {
class HUD {
public:

    struct HUDSquare {
        SDL_Rect tela;
        Color color;
    };

    struct HUDTxt {
        int indexFonte;
        int indexSquare;
        int posX;
        int posY;
        std::string *pText;
        Color color;
    };

    HUD ( const SDL_Rect &_displayArea );

    ~HUD();

    void addFont ( Font *_pFont );
    void addSquare ( const SDL_Rect _rec, Color _color );

    void setOn ( const bool _on ) {
        on = _on;
    }

    bool isOn ( void ) const {
        return on;
    }

    void addText ( int _fontIndex, int _squareIndex, int _posX, int _posY, Color _colorText, std::string *_txt );
    void update ( void );

private:
    void beginOrtho ( void );
    void endOrtho ( void );

    bool on;

    SDL_Rect displayArea;

    std::vector<HUDSquare*> vSquare;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
