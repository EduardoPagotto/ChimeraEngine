#include "Mesh.h"
#include "ChimeraUtils.h"

namespace Chimera {
    
Mesh::Mesh (Node *_parent, std::string _name ) : Draw (_parent, DrawType::MESH, _name ) {

}

Mesh::Mesh ( const Mesh &_cpy ) : Draw ( _cpy ) {

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

void Mesh::init() {
    
    if (pTexture != nullptr) {
        pTexture->init();
        pState->setEnableTexture(pTexture, true);
    }
    
    if (pMaterial == nullptr) {
        pMaterial = new Material("DefaultMat");
    }

    pMaterial->init();
    pState->setEnableMaterial(pMaterial, true);
    
    setVertexBuffer();
    
    Draw::init();
}


btVector3 Mesh::getSizeBox() {
    btVector3 retorno ( 0.0f, 0.0f, 0.0f );
    btVector3 l_max ( 0.0f, 0.0f, 0.0f );
    btVector3 l_min ( 0.0f, 0.0f, 0.0f );

    int l_numVer = vList.getSize() / 3;

    for ( int contador = 0; contador < l_numVer; contador++ ) {
        int indice = contador * 3;
        if ( l_max.x() < vList[indice] ) {
            l_max.setX ( vList[indice] );
        }

        if ( l_max.y() < vList[indice + 1] ) {
            l_max.setY ( vList[indice + 1] );
        }

        if ( l_max.z() < vList[indice + 2] ) {
            l_max.setZ ( vList[indice + 2] );
        }

        if ( l_min.x() > vList[indice] ) {
            l_min.setX ( vList[indice] );
        }

        if ( l_min.y() > vList[indice + 1] ) {
            l_min.setY ( vList[indice + 1] );
        }

        if ( l_min.z() > vList[indice + 2] ) {
            l_min.setZ ( vList[indice + 2] );
        }
    }

    retorno.setValue ( ( btFabs ( l_max.x() ) + btFabs ( l_min.x() ) ) / 2,
                       ( btFabs ( l_max.y() ) + btFabs ( l_min.y() ) ) / 2,
                       ( btFabs ( l_max.z() ) + btFabs ( l_min.z() ) ) / 2 );
    return retorno;
}

void Mesh::renderExecute ( bool _texture ) {
    
	pState->apply();

    if ( _texture == true ) {

		if (pState->getTexture() != nullptr)
			pState->appyTexture();
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		pState->appyMaterial();
          
        unsigned l_numFaces = vIndex.getSize() / 3;
        int l_index = 0;
        int fa = 0;
        for ( unsigned face = 0; face < l_numFaces; face++ ) {
            fa = face * 3;
            glBegin ( GL_TRIANGLES );
            for ( unsigned point = 0; point < 3; point++ ) {
                l_index = fa + point;
                int posicao = 3 * vIndex[l_index];

                int posNormal = 3 * nIndex[l_index];
                glNormal3fv ( &nList[posNormal] );

                if ( tIndex.getSize() > 0 ) {
                    //Ajuste de textura do imageSDL invertendo valor de V
                    int posTex = 2 * tIndex[l_index];
                    float l_U = uvList[posTex];
                    float l_V = uvList[posTex + 1];
                    l_V = 1 - l_V;
                    glTexCoord2f ( l_U, l_V );
                }

                glVertex3fv ( &vList[posicao] );
            }
            glEnd();
        }

    } else {


        unsigned l_numFaces = vIndex.getSize() / 3;
        int l_index = 0;
        int fa = 0;
        for ( unsigned face = 0; face < l_numFaces; face++ ) {
            fa = face * 3;
            glBegin ( GL_TRIANGLES );
            for ( unsigned point = 0; point < 3; point++ ) {
                l_index = fa + point;
                int posicao = 3 * vIndex[l_index];

                int posNormal = 3 * nIndex[l_index];
                glNormal3fv ( &nList[posNormal] );
                glVertex3fv ( &vList[posicao] );
            }
            glEnd();
        }
    }
}

 void Mesh::update ( DataMsg *dataMsg ) {
 
	 if (dataMsg->getKindOp() == KindOp::DRAW) {

		 renderExecute(true);

	 } else if (dataMsg->getKindOp() == KindOp::DRAW_NO_TEX) {

		 renderExecute(false);

	 }

     Draw::update ( dataMsg );
 }

int Mesh::getSource ( tinyxml2::XMLElement* _source, ListPtr<float> &_arrayValores ) {

    tinyxml2::XMLElement* l_nSource = _source->FirstChildElement ( "float_array" );
    if ( l_nSource != nullptr ) {

        const char *l_numCount = l_nSource->Attribute ( "count" );
        if ( l_numCount != nullptr ) {

            std::vector<float> l_array;
            const char* l_vals = l_nSource->GetText();
            loadArrayBtScalar ( l_vals, l_array );
            _arrayValores.create ( l_array.size() );
            for ( unsigned l_contador = 0; l_contador < l_array.size(); l_contador++ ) {
                _arrayValores[l_contador] = l_array[l_contador];
            }

            return _arrayValores.getSize();
        }
    }

    return -1;

}

void Mesh::loadCollada ( tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement* l_nMesh = _nNode->FirstChildElement ( "mesh" );
    if ( l_nMesh != nullptr ) {

        tinyxml2::XMLElement* l_nSource = l_nMesh->FirstChildElement ( "source" );

        while ( l_nSource != nullptr ) {

            const char *l_id = l_nSource->Attribute ( "id" );
            if ( strstr ( l_id, ( char* ) "-positions" ) != nullptr ) {
                getSource ( l_nSource, vList );
            } else if ( strstr ( l_id, ( char* ) "-normals" ) != nullptr ) {
                getSource ( l_nSource, nList );
            } else if ( strstr ( l_id, ( char* ) "-map-0" ) != nullptr ) {
                getSource ( l_nSource, uvList );
            }

            l_nSource = l_nSource->NextSiblingElement ( "source" );
        }

        tinyxml2::XMLElement* l_nPoly = l_nMesh->FirstChildElement ( "polylist" );
        if ( l_nPoly != nullptr ) {

            const char *l_count = l_nPoly->Attribute ( "count" );
            const char *l_mat = l_nPoly->Attribute ( "material" );

            tinyxml2::XMLElement* l_nInput = l_nPoly->FirstChildElement ( "input" );

            std::vector<const char*> l_vOffset;
            std::vector<const char*> l_vSemantic;
            std::vector<const char*> l_vSource;

            while ( l_nInput != nullptr ) {

                const char *l_offSet = l_nInput->Attribute ( "offset" );
                const char *l_semantic = l_nInput->Attribute ( "semantic" );
                const char *l_source = l_nInput->Attribute ( "source" );

                l_vOffset.push_back ( l_offSet );
                l_vSemantic.push_back ( l_semantic );
                l_vSource.push_back ( l_source );

                l_nInput = l_nInput->NextSiblingElement ( "input" );
            }

            tinyxml2::XMLElement* l_nP = l_nPoly->FirstChildElement ( "p" );
            const char *l_indices = l_nP->GetText();
            std::vector<int> l_arrayIndex;

            loadArrayI ( l_indices, l_arrayIndex );

            unsigned l_veCount = 0;
            unsigned l_noCount = 0;
            unsigned l_uvCount = 0;

            int l_numTriangles = atoi ( l_count );

            for ( unsigned l_contador = 0; l_contador < l_arrayIndex.size(); l_contador++ ) {
                int index = l_contador % l_vOffset.size();

                const char *l_offSet = l_vOffset[index];
                const char *l_semantic = l_vSemantic[index];
                const char *l_source = l_vSource[index];

                if ( strstr ( l_source, ( char* ) "-vertices" ) != nullptr ) {
                    if ( vIndex.getSize() == 0 ) {
                        vIndex.create ( l_numTriangles * 3 );
                    }

                    vIndex[l_veCount] = l_arrayIndex[l_contador];
                    l_veCount++;
                } else if ( strstr ( l_source, ( char* ) "-normals" ) != nullptr ) {
                    if ( nIndex.getSize() == 0 ) {
                        nIndex.create ( l_numTriangles * 3 );
                    }

                    nIndex[l_noCount] = l_arrayIndex[l_contador];
                    l_noCount++;
                } else if ( strstr ( l_source, ( char* ) "-map-0" ) != nullptr ) {
                    if ( tIndex.getSize() == 0 ) {
                        tIndex.create ( l_numTriangles * 3 );
                    }

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

void Mesh::setVertexBuffer()
{
//     //Ajusta Buffer de textura com imagem SDL
//     int tamanho = uvList.getSize() / 2;
//     for(int indice = 0; indice < tamanho; indice += 2) {
//         float l_v = uvList[indice + 1];
//         l_v = 1 - l_v;
//         uvList[indice + 1] = l_v;
//     }
    
    
    printf("Nome: %s \n", getName().c_str());
    int linha = 0;
    printf("-Vertex Indice ----------( %03d )\n", vIndex.getSize());
    for (int indice=0 ; indice < vIndex.getSize(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, vIndex[indice], vIndex[indice + 1], vIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Vertex Lista ---------( %03d )\n", vList.getSize());
    for (int indice=0 ; indice < vList.getSize(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %0.0f ; %0.0f ; %0.0f )\n",linha, indice, vList[indice], vList[indice + 1], vList[indice + 2]);
        linha++;
    }
    printf("\n");
    

    linha = 0;
    printf("Normal Indice ----------( %03d )\n", nIndex.getSize());
    for (int indice=0 ; indice < nIndex.getSize(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, nIndex[indice], nIndex[indice + 1], nIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Normal Lista ---------( %03d )\n", nList.getSize());
    for (int indice=0 ; indice < nList.getSize(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %0.0f ; %0.0f ; %0.0f )\n",linha, indice, nList[indice], nList[indice + 1], nList[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("Texture Indice ----------( %03d )\n", tIndex.getSize());
    for (int indice=0 ; indice < tIndex.getSize(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, tIndex[indice], tIndex[indice + 1], tIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Texture Lista ---------( %03d )\n", uvList.getSize());
    for (int indice=0 ; indice < uvList.getSize(); indice += 2) {
        printf("Linha: %02d : p: %02d ( %0.0f ; %0.0f )\n",linha, indice, uvList[indice], uvList[indice + 1]);
        linha++;
    }
    printf("\n");
}

//TODO trocar quando VBO estiver funcional no init
//     VertexVBOID = 0;
//     IndexVBOID = 0;
//     
//     glGenBuffers(1, &VertexVBOID);
//     glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
//     glBufferData(GL_ARRAY_BUFFER, vList.getSize() ,   &vList[0] , GL_STATIC_DRAW);
//     
//     glGenBuffers(1, &IndexVBOID);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndex.getSize(), &vIndex[0], GL_STATIC_DRAW);

//TODO trocar quando VBO estiver funcional no render
//         //glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
//         glEnableClientState(GL_VERTEX_ARRAY);
//         glVertexPointer(3, GL_FLOAT, 0, &vList[0] );   //The starting point of the VBO, for the vertices
//                 
//         glEnableClientState(GL_NORMAL_ARRAY);
//         glNormalPointer(GL_FLOAT, 0, &pNorm[0] );   //The starting point of normals, 12 bytes away
//         
//         if (pState->getTexture() != nullptr) {
//             glClientActiveTexture(GL_TEXTURE0);
//             glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//             glTexCoordPointer(2, GL_FLOAT, 0, &pTex[0] );   //The starting point of texcoords, 24 bytes away
//         }
//         
//         //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
//         glDrawElements(GL_TRIANGLES, vIndex.getSize(), GL_UNSIGNED_INT, &vIndex[0]);   //The starting point of the IBO
//   
//         glDisableClientState(GL_VERTEX_ARRAY);
//         
//         if (pState->getTexture() != nullptr) {
//             glDisableClientState(GL_TEXTURE0);
//         }
//         
//         glDisableClientState(GL_NORMAL_ARRAY);

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
