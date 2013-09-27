#include "Mesh.h"

namespace Chimera {

Mesh::Mesh() {
    m_typeDraw = TRI_MESH;
    m_pMaterial = NULL;
}

Mesh::Mesh ( const Mesh &_cpy ) {
    m_typeDraw = TRI_MESH;

    m_vertexList.set ( _cpy.m_vertexList );
    m_normalList.set ( _cpy.m_normalList );
    m_uv.set ( _cpy.m_uv );

    m_vertexIndex.set ( _cpy.m_vertexIndex );
    m_normalIndex.set ( _cpy.m_normalIndex );
    m_textureIndex.set ( _cpy.m_textureIndex );

    m_pMaterial = _cpy.m_pMaterial;
}

Mesh::~Mesh() {
    m_vertexList.clear();
    m_normalList.clear();
    m_uv.clear();

    m_vertexIndex.clear();
    m_normalIndex.clear();
    m_textureIndex.clear();
}

btVector3 Mesh::sizeQuadratic ( void ) {
    btVector3 retorno ( 0.0f,0.0f,0.0f );
    btVector3 l_max ( 0.0f,0.0f,0.0f );
    btVector3 l_min ( 0.0f,0.0f,0.0f );

    int l_numVer = m_vertexList.size() / 3;

    for ( int contador=0; contador < l_numVer ; contador++ ) {
        int indice = contador*3;
        if ( l_max.x() < m_vertexList[indice] )
            l_max.setX ( m_vertexList[indice] );

        if ( l_max.y() < m_vertexList[indice+1] )
            l_max.setY ( m_vertexList[indice+1] );

        if ( l_max.z() < m_vertexList[indice+2] )
            l_max.setZ ( m_vertexList[indice+2] );

        if ( l_min.x() > m_vertexList[indice] )
            l_min.setX ( m_vertexList[indice] );

        if ( l_min.y() > m_vertexList[indice+1] )
            l_min.setY ( m_vertexList[indice+1] );

        if ( l_min.z() > m_vertexList[indice+2] )
            l_min.setZ ( m_vertexList[indice+2] );
    }

    retorno.setValue ( ( btFabs ( l_max.x() ) + btFabs ( l_min.x() ) ) /2,
                       ( btFabs ( l_max.y() ) + btFabs ( l_min.y() ) ) /2,
                       ( btFabs ( l_max.z() ) + btFabs ( l_min.z() ) ) /2 );
    return retorno;
}

void Mesh::renderizar ( void ) {
    unsigned l_numFaces = m_vertexIndex.size() / 3;
    int l_index = 0;
    int fa = 0;
    for ( unsigned face=0 ; face < l_numFaces ; face++ ) {
        fa = face * 3;
        glBegin ( GL_TRIANGLES );
        for ( unsigned point=0 ; point < 3 ; point++ ) {
            l_index = fa + point;
            int posicao = 3 * m_vertexIndex[ l_index ];

            int posNormal = 3 * m_normalIndex[ l_index ];
            glNormal3fv ( &m_normalList[ posNormal ] );

            if ( m_textureIndex.size() > 0 ) {
                //Ajuste de textura do imageSDL invertendo valor de V
                int posTex = 2 * m_textureIndex[ l_index ];
                float l_U = m_uv[ posTex ];
                float l_V = m_uv[ posTex +1 ];
                l_V = 1 - l_V;
                glTexCoord2f ( l_U, l_V );
            }

            glVertex3fv ( &m_vertexList[ posicao ] );
        }
        glEnd();
    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
