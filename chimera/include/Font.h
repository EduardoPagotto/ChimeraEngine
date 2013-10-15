#ifndef FONT_H_
#define FONT_H_


#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include <FTGL/ftgl.h>

#include "Exception.h"
#include "Color.h"

namespace Chimera {

#define CHIMERA_MAX_LINE_SIZE 512

class Font {
public:
    Font ( const char*_fontFile, int _size );
    virtual ~Font ( void );

    bool getStatus ( void ) const {
        return m_status;
    }

    void setStatus ( bool _status ) {
        m_status = _status;
    }

    void render2 ( float _x, float _y, Color _color, const char *_txt );
    void render ( float _x, float _y, Color _color, const char *_fmt, ... );
    void render ( float _x, float _y, Color _color, std::string *_txt );

private:
    bool status;
    char txt[CHIMERA_MAX_LINE_SIZE];
    FTGLPixmapFont *pFont;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
