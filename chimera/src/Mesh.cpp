#include "Mesh.h"

namespace Chimera {

Mesh::Mesh() : Draw(DrawType::MESH) {
    
}

Mesh::Mesh ( const Mesh &_cpy ): Draw(_cpy) {

    vList.set ( _cpy.vList );
    nList.set ( _cpy.nList );
    uvList.set ( _cpy.uvList );

    vIndex.set ( _cpy.vIndex );
    nIndex.set ( _cpy.nIndex );
    tIndex.set ( _cpy.tIndex );

}

Mesh::~Mesh() {
    vList.clear();
    nList.clear();
    uvList.clear();

    vIndex.clear();
    nIndex.clear();
    tIndex.clear();
}

btVector3 Mesh::getSizeBox () {
    btVector3 retorno ( 0.0f,0.0f,0.0f );
    btVector3 l_max ( 0.0f,0.0f,0.0f );
    btVector3 l_min ( 0.0f,0.0f,0.0f );

    int l_numVer = vList.getSize() / 3;

    for ( int contador=0; contador < l_numVer ; contador++ ) {
        int indice = contador*3;
        if ( l_max.x() < vList[indice] )
            l_max.setX ( vList[indice] );

        if ( l_max.y() < vList[indice+1] )
            l_max.setY ( vList[indice+1] );

        if ( l_max.z() < vList[indice+2] )
            l_max.setZ ( vList[indice+2] );

        if ( l_min.x() > vList[indice] )
            l_min.setX ( vList[indice] );

        if ( l_min.y() > vList[indice+1] )
            l_min.setY ( vList[indice+1] );

        if ( l_min.z() > vList[indice+2] )
            l_min.setZ ( vList[indice+2] );
    }

    retorno.setValue ( ( btFabs ( l_max.x() ) + btFabs ( l_min.x() ) ) /2,
                       ( btFabs ( l_max.y() ) + btFabs ( l_min.y() ) ) /2,
                       ( btFabs ( l_max.z() ) + btFabs ( l_min.z() ) ) /2 );
    return retorno;
}

void Mesh::init() {
      
}

void Mesh::render() {

    unsigned l_numFaces = vIndex.getSize() / 3;
    int l_index = 0;
    int fa = 0;
    for ( unsigned face=0 ; face < l_numFaces ; face++ ) {
        fa = face * 3;
        glBegin ( GL_TRIANGLES );
        for ( unsigned point=0 ; point < 3 ; point++ ) {
            l_index = fa + point;
            int posicao = 3 * vIndex[ l_index ];
            
            int posNormal = 3 * nIndex[ l_index ];
            glNormal3fv ( &nList[ posNormal ] );
            
            if ( tIndex.getSize() > 0 ) {
                //Ajuste de textura do imageSDL invertendo valor de V
                int posTex = 2 * tIndex[ l_index ];
                float l_U = uvList[ posTex ];
                float l_V = uvList[ posTex +1 ];
                l_V = 1 - l_V;
                glTexCoord2f ( l_U, l_V );
            }
            
            glVertex3fv ( &vList[ posicao ] );
        }
        glEnd();
    }   
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
