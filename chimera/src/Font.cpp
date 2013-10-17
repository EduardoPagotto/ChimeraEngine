#include "Font.h"

namespace Chimera {

Font::Font ( const char* _fontFile, int _size ) {
    if ( _fontFile == nullptr )
        CHIMERA_THROW ( 99,"Arquivo de Fonte Nulo" );

    m_pFont = new FTGLPixmapFont ( _fontFile );
    if ( m_pFont==nullptr )
        CHIMERA_THROW ( 99,"Carga de arquivo invalida" );

    if ( m_pFont->Error() )
        CHIMERA_THROW ( 99,"Carga de Fonte invalida" );

    if ( m_pFont->FaceSize ( _size ) ==false ) {
        if ( m_pFont ) {
            delete m_pFont;
            m_pFont = nullptr;
        }
        CHIMERA_THROW ( 99,"Tamanho Fonte invalida" );
    }

    status = true;
    memset ( txt,0,CHIMERA_MAX_LINE_SIZE );
}

Font::~Font ( void ) {
    if ( pFont ) {
        delete pFont;
        pFont = nullptr;
    }
}

void Font::render ( float _x, float _y, Color _color, const char *_fmt, ... ) {
    if ( status==true ) {
        va_list args;
        va_start ( args, _fmt );

        glColor3f ( _color.r, _color.g , _color.b );
        glRasterPos2f ( _x, _y );

        _vsnprintf_s ( txt,CHIMERA_MAX_LINE_SIZE,_TRUNCATE, _fmt, args );
        pFont->Render ( txt );

        va_end ( args );
    }
}

void Font::render2 ( float _x, float _y, Color _color, const char *_txt ) {
    glColor3f ( _color.r, _color.g , _color.b );
    glRasterPos2f ( _x, _y );
    pFont->Render ( _txt );
}


void Font::render ( float _x, float _y, Color _color, std::string *_txt ) {
    glColor3f ( _color.r, _color.g , _color.b );
    glRasterPos2f ( _x, _y );
    pFont->Render ( _txt->c_str() );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
