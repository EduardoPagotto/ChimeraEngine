#ifndef FONT_H_
#define FONT_H_

//#include <GL/gl.h>			// Header File For The OpenGL32 Library
//#include <GL/glu.h>			// Header File For The GLu32 Library

//#include <FTGL/ftgl.h>

#include "ExceptionChimera.h"
#include "Color.h"

namespace Chimera {

class Font {
public:
    Font ( const char*_fontFile, int _size );
    virtual ~Font ( void );

    void render (const float &_x, const float &_y,const Color &_color, std::string *_pTxt);

private:
   // FTGLPixmapFont *pFont;
    
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
