#ifndef FONT_H_
#define FONT_H_

#define TTF_NOVO

#ifdef TTF_NOVO

#ifndef WIN32
#include <SDL2/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif

#else
#include <FTGL/ftgl.h>
#endif

#include "ExceptionChimera.h"
#include "Color.h"

namespace Chimera {

class Font {
public:
    Font ( const char*_fontFile, int _size );
    virtual ~Font ( void );

    void render (const float &_x, const float &_y,const float &_z ,const Color &_color, std::string *_pTxt);

private:
    
#ifdef TTF_NOVO
    TTF_Font *pFont;
#else
    FTGLPixmapFont *pFont;
#endif
   
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
