#include "Font.h"

#include <ExceptionSDL.h>

#ifdef WIN32
#include "windows.h"
#endif
#include <GL/gl.h>

namespace Chimera {

Font::Font ( const char* _fontFile, int _size ) {
    
#ifdef TTF_NOVO 
    
    pFont = TTF_OpenFont( _fontFile, _size );
    if(!pFont)
       throw ExceptionSDL ( ExceptionCode::READ, "Arquivo de fonte falha ao carregar:" + std::string(_fontFile ) );
    
    TTF_SetFontStyle(pFont, TTF_STYLE_NORMAL);
    
#else  
    if ( _fontFile == nullptr )
       ExceptionChimera ( ExceptionCode::READ,"Arquivo de Fonte Nulo" );

    pFont = new FTGLPixmapFont ( _fontFile );
    if ( pFont==nullptr )
       ExceptionChimera ( ExceptionCode::READ,"Carga de arquivo invalida" );

    if ( pFont->Error() )
       ExceptionChimera ( ExceptionCode::READ,"Carga de Fonte invalida" );

    if ( pFont->FaceSize ( _size ) ==false ) {
       if ( pFont ) {
           delete pFont;
           pFont = nullptr;
       }
       ExceptionChimera ( ExceptionCode::READ,"Tamanho Fonte invalida" );
    }
    
#endif 
}

Font::~Font ( void ) {
    
    if ( pFont != nullptr ) {
       delete pFont;
       pFont = nullptr;
    }
    
}

void Font::render (const float &_x, const float &_y,const float &_z ,const Color &_color, std::string *_pTxt ) {
   
#ifdef TTF_NOVO 
        SDL_Color Color = {_color.r , _color.g , _color.b};
        SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(pFont), _pTxt->c_str(), Color);
        unsigned Texture = 0;
 
        /*Generate an OpenGL 2D texture from the SDL_Surface*.*/
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
 
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0,GL_BGRA_EXT,
                     GL_UNSIGNED_BYTE, Message->pixels);
        
        /*Draw this texture on a quad with the given xyz coordinates.*/
         glBegin(GL_QUADS);
                 glTexCoord2d(0, 1); glVertex3d(_x, _y, _z);
                 glTexCoord2d(1, 1); glVertex3d(_x+Message->w, _y, _z);
                 glTexCoord2d(1, 0); glVertex3d(_x+Message->w, _y+Message->h, _z);
                 glTexCoord2d(0, 0); glVertex3d(_x, _y+Message->h, _z);
         glEnd();

        //glBegin(GL_QUADS);
        //        glTexCoord2d(0, 0); glVertex3d(_x, _y, _z);
        //        glTexCoord2d(1, 0); glVertex3d(_x+Message->w, _y, _z);
        //        glTexCoord2d(1, 1); glVertex3d(_x+Message->w, _y+Message->h, _z);
        //        glTexCoord2d(0, 1); glVertex3d(_x, _y+Message->h, _z);
        //glEnd();
 
        /*Clean up.*/
        glDeleteTextures(1, &Texture);
        SDL_FreeSurface(Message);
#else  
    
    glColor3fv ( _color.ptr() );
    glRasterPos2f ( _x, _y );
    pFont->Render ( _pTxt->c_str(), _pTxt->size() );
    
#endif 
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
