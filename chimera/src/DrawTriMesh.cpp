#include "DrawTriMesh.h"

namespace Chimera {

DrawTriMesh::DrawTriMesh(std::string _id, std::string _name) : Draw(DrawType::MESH,_id,_name) {  
}

DrawTriMesh::DrawTriMesh ( const DrawTriMesh &_cpy ): Draw(_cpy) {

    vList.set ( _cpy.vList );
    nList.set ( _cpy.nList );
    uvList.set ( _cpy.uvList );

    vIndex.set ( _cpy.vIndex );
    nIndex.set ( _cpy.nIndex );
    tIndex.set ( _cpy.tIndex );

}

DrawTriMesh::~DrawTriMesh() {
    vList.clear();
    nList.clear();
    uvList.clear();

    vIndex.clear();
    nIndex.clear();
    tIndex.clear();
}

btVector3 DrawTriMesh::getSizeBox () {
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

void DrawTriMesh::init() {
      
}

void DrawTriMesh::render() {

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

void DrawTriMesh::clone(Node **ppNode ) {
    *ppNode = new DrawTriMesh( *this ); 
    Draw::clone( ppNode );  
}

void DrawTriMesh::update ( DataMsg *dataMsg ) {
    Node::update(dataMsg);
}

int DrawTriMesh::getSource (tinyxml2::XMLElement* _source , ListPtr<float> &_arrayValores ) {
    
    tinyxml2::XMLElement* l_nSource = _source->FirstChildElement("float_array");
    if (l_nSource != nullptr) {
        
        const char *l_numCount = l_nSource->Attribute("count");
        if (l_numCount != nullptr) {
            
            std::vector<float> l_array;
            const char* l_vals = l_nSource->GetText();
            loadArrayBtScalar(l_vals,l_array);
            _arrayValores.create( l_array.size() );
            for ( unsigned l_contador = 0 ; l_contador < l_array.size() ; l_contador++ )
                _arrayValores[l_contador] = l_array[l_contador];           
            
            return _arrayValores.getSize();
        }
    }
    
    return -1;
    
}

void DrawTriMesh::loadCollada(tinyxml2::XMLElement* _nNode) {
    
    tinyxml2::XMLElement* l_nMesh = _nNode->FirstChildElement ( "mesh" );
    if (l_nMesh != nullptr) {
        
        tinyxml2::XMLElement* l_nSource = l_nMesh->FirstChildElement ( "source" );
        
        while (l_nSource != nullptr) {
            
            const char *l_id = l_nSource->Attribute("id");
            if ( strstr ( l_id, ( char* ) "-positions" ) != nullptr ) { 
                getSource ( l_nSource , vList ); 
            } else if ( strstr ( l_id, ( char* ) "-normals" ) != nullptr ) {
                getSource ( l_nSource , nList );
            } else if ( strstr ( l_id, ( char* ) "-map-0" ) != nullptr ) {
                getSource ( l_nSource , uvList );
            }
            
            l_nSource = l_nSource->NextSiblingElement("source");
        }
        
        tinyxml2::XMLElement* l_nPoly = l_nMesh->FirstChildElement ( "polylist" );
        if (l_nPoly != nullptr) {
            
            const char *l_count = l_nPoly->Attribute("count");
            const char *l_mat = l_nPoly->Attribute("material");
            
            tinyxml2::XMLElement* l_nInput = l_nPoly->FirstChildElement ( "input" );
            
            std::vector<const char*> l_vOffset;
            std::vector<const char*> l_vSemantic;
            std::vector<const char*> l_vSource;
            
            while (l_nInput != nullptr) {
                
                const char *l_offSet = l_nInput->Attribute("offset"); 
                const char *l_semantic = l_nInput->Attribute("semantic");
                const char *l_source = l_nInput->Attribute("source");
                
                l_vOffset.push_back ( l_offSet );
                l_vSemantic.push_back ( l_semantic );
                l_vSource.push_back ( l_source );
                
                l_nInput = l_nInput->NextSiblingElement("input");
            }
            
            tinyxml2::XMLElement* l_nP = l_nPoly->FirstChildElement ( "p" );
            const char *l_indices = l_nP->GetText();
            std::vector<int> l_arrayIndex;
            
            loadArrayI ( l_indices,l_arrayIndex );
            
            unsigned l_veCount = 0;
            unsigned l_noCount = 0;
            unsigned l_uvCount = 0;
            
            int l_numTriangles = atoi (l_count);
            
            for ( unsigned l_contador = 0 ; l_contador < l_arrayIndex.size() ; l_contador++ ) {
                int index = l_contador % l_vOffset.size();
                
                const char *l_offSet = l_vOffset[index];
                const char *l_semantic = l_vSemantic[index];
                const char *l_source = l_vSource[index];
                
                if ( strstr ( l_source, ( char* ) "-vertices" ) != nullptr ) {
                    if ( vIndex.getSize() == 0 )
                        vIndex.create ( l_numTriangles * 3 );
                    
                    vIndex[l_veCount] = l_arrayIndex[l_contador];
                    l_veCount++;
                } else if ( strstr ( l_source, ( char* ) "-normals" ) != nullptr ) {
                    if ( nIndex.getSize() ==0 )
                        nIndex.create ( l_numTriangles * 3 );
                    
                    nIndex[l_noCount] = l_arrayIndex[l_contador];
                    l_noCount++;
                } else if ( strstr ( l_source, ( char* ) "-map-0" ) != nullptr ) {
                    if ( tIndex.getSize() ==0 )
                        tIndex.create ( l_numTriangles* 3 );
                    
                    tIndex[l_uvCount] = l_arrayIndex[l_contador];
                    l_uvCount++;
                }
            }
            l_arrayIndex.clear(); 
            
            l_vOffset.clear();
            l_vSemantic.clear();
            l_vSource.clear();
            
        }
        
    }
    
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
