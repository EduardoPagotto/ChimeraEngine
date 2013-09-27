#ifndef HUD_H_
#define HUD_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
#include <vector>

#include "Font.h"
//#include "ChimeraGenericTypes.h"

namespace Chimera {
class HUD {
public:

    struct HUDSquare {
        Rectangular tela;
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

    HUD ( const Rectangular _rec );

    ~HUD();

    void addFont ( Font *_pFont );
    void addSquare ( const Rectangular _rec, Color _color );

    void setStatus ( const bool _status ) {
        m_status = _status;
    }

    bool getStatus ( void ) const {
        return m_status;
    }

    void addText ( int _fontIndex, int _squareIndex, int _posX, int _posY, Color _colorText, std::string *_txt );
    void update ( void );

private:
    void beginOrtho ( void );
    void endOrtho ( void );

    bool m_status;

    Rectangular m_tela;

    std::vector<HUDSquare*> m_vpSquare;
    std::vector<HUDTxt*> m_vText;
    std::vector<Font*> m_vpFont;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
