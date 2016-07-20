#include "Mesh.h"
#include "ChimeraUtils.h"

#include "VboIndexer.hpp"

namespace Chimera {
    
Mesh::Mesh (Node *_parent, std::string _name ) : Draw (_parent, DrawType::MESH, _name ) {

}

Mesh::Mesh ( const Mesh &_cpy ) : Draw ( _cpy ) {

//     vList.set ( _cpy.vList );
//     nList.set ( _cpy.nList );
//     uvList.set ( _cpy.uvList );
// 
//     vIndex.set ( _cpy.vIndex );
//     nIndex.set ( _cpy.nIndex );
//     tIndex.set ( _cpy.tIndex );

}

Mesh::~Mesh() {

    vertexIndex.clear();
    vertexList.clear();
    normalIndex.clear();
    normalList.clear();
    textureIndex.clear();
    textureList.clear();
   
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

    for ( int indice = 0; indice < vertexList.size(); indice++ ) {
        
        if ( l_max.x() < vertexList[indice].x )
            l_max.setX ( vertexList[indice].x );
      
        if ( l_max.y() < vertexList[indice].y )
            l_max.setY ( vertexList[indice].y );

        if ( l_max.z() < vertexList[indice].z )
            l_max.setZ ( vertexList[indice].z );

        if ( l_min.x() > vertexList[indice].x )
            l_min.setX ( vertexList[indice].x );

        if ( l_min.y() > vertexList[indice].y )
            l_min.setY ( vertexList[indice].y );

        if ( l_min.z() > vertexList[indice].z )
            l_min.setZ ( vertexList[indice].z );
    }

    retorno.setValue ( ( btFabs ( l_max.x() ) + btFabs ( l_min.x() ) ) / 2,
                       ( btFabs ( l_max.y() ) + btFabs ( l_min.y() ) ) / 2,
                       ( btFabs ( l_max.z() ) + btFabs ( l_min.z() ) ) / 2 );
    return retorno;
}

void Mesh::renterTeste() {
    
        if (pState->getTexture() != nullptr)
            pState->appyTexture();
        else 
            glBindTexture(GL_TEXTURE_2D, 0);
       
        pState->appyMaterial();
    
    //        glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
       glEnableClientState(GL_VERTEX_ARRAY);
       //glVertexPointer(3, GL_FLOAT, 0, &vertexList[0] ); 
       glVertexPointer(3, GL_FLOAT, 0, &vertexVBO[0] );   //The starting point of the VBO, for the vertices
                
//         glEnableClientState(GL_NORMAL_ARRAY);
//         glNormalPointer(GL_FLOAT, 0, &normalVBO[0] );   //The starting point of normals, 12 bytes away
//         
//         if (textureVBO.size() > 0) {
//             glClientActiveTexture(GL_TEXTURE0);
//             glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//             glTexCoordPointer(2, GL_FLOAT, 0, &textureVBO[0] );   //The starting point of texcoords, 24 bytes away
//         }
        
 //       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
        //glDrawElements(GL_TRIANGLES, vertexIndex.size(), GL_UNSIGNED_INT, &vertexIndex[0]);
        glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, &indexIBO[0]);   //The starting point of the IBO
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
//         if (pState->getTexture() != nullptr) {
//             glDisableClientState(GL_TEXTURE0);
//         }
//         
//         glDisableClientState(GL_NORMAL_ARRAY);
}

void Mesh::renderExecute ( bool _texture ) {

    pState->apply();
    if ( _texture == true ) {
        
        if (pState->getTexture() != nullptr)
            pState->appyTexture();
        else 
            glBindTexture(GL_TEXTURE_2D, 0);
       
        pState->appyMaterial();
                    
        
        unsigned l_numFaces = vertexIndex.size() / 3;
        int l_index = 0;
        int fa = 0;
        for ( unsigned face = 0; face < l_numFaces; face++ ) {
            fa = face * 3;
            
            glBegin ( GL_TRIANGLES );
            
            for ( unsigned point = 0; point < 3; point++ ) {
                l_index = fa + point;
                int posicao =  vertexIndex[l_index];
                int posNormal =  normalIndex[l_index];
                
                glNormal3fv ( (GLfloat*) &normalList[posNormal] );

                if ( textureIndex.size() > 0 ) 
                    glTexCoord2fv( (GLfloat*)&textureList[ textureIndex[l_index] ]  );
                
                glVertex3fv ( (GLfloat*) &vertexList[posicao] );
            }
            
            glEnd();
        }

    } else {

        unsigned l_numFaces = vertexIndex.size() / 3;
        int l_index = 0;
        int fa = 0;
        for ( unsigned face = 0; face < l_numFaces; face++ ) {
            fa = face * 3;
            glBegin ( GL_TRIANGLES );
            for ( unsigned point = 0; point < 3; point++ ) {
                l_index = fa + point;
                int posicao =  vertexIndex[l_index];
                int posNormal =  normalIndex[l_index];
                glNormal3fv ( (GLfloat*) &normalList[posNormal] );
                glVertex3fv ( (GLfloat*) &vertexList[posicao] );
            }
            glEnd();
        }
    }    
}

 void Mesh::update ( DataMsg *dataMsg ) {
 
	 if (dataMsg->getKindOp() == KindOp::DRAW) {

		 //renderExecute(true);
         renterTeste();

	 } else if (dataMsg->getKindOp() == KindOp::DRAW_NO_TEX) {

		 renderExecute(false);

	 }

     Draw::update ( dataMsg );
 }

int Mesh::getSource ( tinyxml2::XMLElement* _source, std::vector<float> &_arrayValores ) {

    tinyxml2::XMLElement* l_nSource = _source->FirstChildElement ( "float_array" );
    if ( l_nSource != nullptr ) {

        const char *l_numCount = l_nSource->Attribute ( "count" );
        if ( l_numCount != nullptr ) {

            //std::vector<float> l_array;
            const char* l_vals = l_nSource->GetText();
            loadArrayBtScalar( l_vals, _arrayValores );
            return _arrayValores.size();
        }
    }

    return -1;

}

void Mesh::loadCollada ( tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement* l_nMesh = _nNode->FirstChildElement ( "mesh" );
    if ( l_nMesh != nullptr ) {

        tinyxml2::XMLElement* l_nSource = l_nMesh->FirstChildElement ( "source" );

        //Carrega lista de vetores
        while ( l_nSource != nullptr ) {

            const char *l_id = l_nSource->Attribute ( "id" );
            if ( strstr ( l_id, ( char* ) "-positions" ) != nullptr ) {
                
                //Carrega todos os vetores ponto
                std::vector<float> lista;
                getSource ( l_nSource, lista );
                
                for (int indice=0; indice < lista.size(); indice += 3) 
                    vertexList.push_back( glm::vec3(lista[indice],lista[indice+1],lista[indice+2]) );
                   
            } else if ( strstr ( l_id, ( char* ) "-normals" ) != nullptr ) {
                
                //carrega todos os vetores normal
                std::vector<float> lista;
                getSource ( l_nSource, lista );

                for (int indice=0; indice < lista.size(); indice += 3) 
                    normalList.push_back( glm::vec3(lista[indice],lista[indice+1],lista[indice+2]));
                  
            } else if ( strstr ( l_id, ( char* ) "-map-0" ) != nullptr ) {
                
                //carrega vetor posicao textura
                std::vector<float> lista;
                getSource ( l_nSource, lista );
                for (int indice=0; indice < lista.size(); indice += 2)
                    textureList.push_back( glm::vec2(lista[indice],lista[indice+1]) );

            }

            l_nSource = l_nSource->NextSiblingElement ( "source" );
        }

        //Carrega Lista de indices
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

            int l_numTriangles = atoi ( l_count );

            for ( unsigned l_contador = 0; l_contador < l_arrayIndex.size(); l_contador++ ) {
                int index = l_contador % l_vOffset.size();

                const char *l_offSet = l_vOffset[index];
                const char *l_semantic = l_vSemantic[index];
                const char *l_source = l_vSource[index];

                if ( strstr ( l_source, ( char* ) "-vertices" ) != nullptr ) { //indices de vetor ponto
                    
                    vertexIndex.push_back( l_arrayIndex[l_contador]  );
                    
                } else if ( strstr ( l_source, ( char* ) "-normals" ) != nullptr ) { //indice de vetor normal
                    
                    normalIndex.push_back(l_arrayIndex[l_contador]);
                    
                } else if ( strstr ( l_source, ( char* ) "-map-0" ) != nullptr ) { //indice de vetor posicao textura
                    
                    textureIndex.push_back( l_arrayIndex[l_contador] );
                    
                }
            }
            l_arrayIndex.clear();

            l_vOffset.clear();
            l_vSemantic.clear();
            l_vSource.clear();

        }
    }
    
    printf("Nome: %s \n", getName().c_str());
    printf("-Vertex Indice ------( %03d )\n", vertexIndex.size());
    printf("-Vertex Lista -------( %03d )\n", vertexList.size());
    printf("-Normal Indice ------( %03d )\n", normalIndex.size());
    printf("-Normal Lista -------( %03d )\n", normalList.size());
    printf("-Texture Indice -----( %03d )\n", textureIndex.size());
    printf("-Texture Lista ------( %03d )\n", textureList.size());
    printf("\n");
}

void Mesh::setVertexBuffer()
{
    //Ajuste de textura do imageSDL invertendo valor de V
    int tamanho = textureIndex.size();
    for(int indice = 0; indice < tamanho; indice++) {
        int posTex =  textureIndex[indice];
        textureList[posTex].y = 1 - textureList[posTex].y;
    }
    
    if (textureList.size() > 0)
        indexVBO(vertexList, textureList, normalList, indexIBO, vertexVBO, textureVBO, normalVBO);
    else
        indexVBOnoTexture(vertexList, normalList, indexIBO, vertexVBO, normalVBO);
    
    printf("Nome: %s \n", getName().c_str());
    printf("-Vertex Indice ------( %03d )\n", indexIBO.size());
    printf("-Vertex Lista -------( %03d )\n", vertexVBO.size());
    printf("-Normal Lista -------( %03d )\n", normalVBO.size());
    printf("-Texture Lista ------( %03d )\n", textureVBO.size());
    printf("\n");
    
    //TODO trocar quando VBO estiver funcional no init
//     VertexVBOID = 0;
//     IndexVBOID = 0;
//     
//     glGenBuffers(1, &VertexVBOID);
//     glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
//     glBufferData(GL_ARRAY_BUFFER, vertexVBO.size() , &vertexVBO[0] , GL_STATIC_DRAW);
//     
//     glGenBuffers(1, &IndexVBOID);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexIBO.size(), &indexIBO[0], GL_STATIC_DRAW);
    
}

void Mesh::debugDados() {
    
    printf("Nome: %s \n", getName().c_str());
    int linha = 0;
    printf("-Vertex Indice ----------( %03d )\n", vertexIndex.size());
    for (int indice=0 ; indice < vertexIndex.size(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, vertexIndex[indice], vertexIndex[indice + 1], vertexIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Vertex Lista ---------( %03d )\n", vertexList.size());
    for (int indice=0 ; indice < vertexList.size(); indice++) {
        printf("Linha: %02d : p: %02d ( %0.0f ; %0.0f ; %0.0f )\n",linha, indice, vertexList[indice].x, vertexList[indice].y, vertexList[indice].z);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("Normal Indice ----------( %03d )\n", normalIndex.size());
    for (int indice=0 ; indice < normalIndex.size(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, normalIndex[indice], normalIndex[indice + 1], normalIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Normal Lista ---------( %03d )\n", normalList.size());
    for (int indice=0 ; indice < normalList.size(); indice++) {
        printf("Linha: %02d : p: %02d ( %0.0f ; %0.0f ; %0.0f )\n",linha, indice, normalList[indice].x, normalList[indice].y, normalList[indice].z);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("Texture Indice ----------( %03d )\n", textureIndex.size());
    for (int indice=0 ; indice < textureIndex.size(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, textureIndex[indice], textureIndex[indice + 1], textureIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Texture Lista ---------( %03d )\n", textureList.size());
    for (int indice=0 ; indice < textureList.size(); indice++) {
        printf("Linha: %02d : p: %02d ( %0.0f ; %0.0f )\n",linha, indice, textureList[indice].x, textureList[indice].y);
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
