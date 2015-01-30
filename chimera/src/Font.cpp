#include "Font.h"

namespace Chimera {

Font::Font ( const char* _fontFile, int _size ) {
    //if ( _fontFile == nullptr )
    //    ExceptionChimera ( ExceptionCode::READ,"Arquivo de Fonte Nulo" );

    //pFont = new FTGLPixmapFont ( _fontFile );
    //if ( pFont==nullptr )
    //    ExceptionChimera ( ExceptionCode::READ,"Carga de arquivo invalida" );

    //if ( pFont->Error() )
    //    ExceptionChimera ( ExceptionCode::READ,"Carga de Fonte invalida" );

    //if ( pFont->FaceSize ( _size ) ==false ) {
    //    if ( pFont ) {
    //        delete pFont;
    //        pFont = nullptr;
    //    }
    //    ExceptionChimera ( ExceptionCode::READ,"Tamanho Fonte invalida" );
    //}
}

Font::~Font ( void ) {
    
    //if ( pFont != nullptr ) {
    //    delete pFont;
    //    pFont = nullptr;
    //}
    
}

void Font::render (const float &_x, const float &_y,const Color &_color, std::string *_pTxt ) {
    
    //glColor3fv ( _color.ptr() );
    //glRasterPos2f ( _x, _y );
    //pFont->Render ( _pTxt->c_str(), _pTxt->size() );
    
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
