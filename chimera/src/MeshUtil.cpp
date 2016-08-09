#include "MeshUtil.h"

namespace Chimera {

void debugDados(Mesh *pMesh) {
    
    printf("Nome: %s \n", pMesh->getName().c_str());
    int linha = 0;
    printf("-Vertex Indice ----------( %03d )\n", pMesh->vertexIndex.size());
    for (unsigned int indice=0 ; indice < pMesh->vertexIndex.size(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, pMesh->vertexIndex[indice], pMesh->vertexIndex[indice + 1], pMesh->vertexIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Vertex Lista ---------( %03d )\n", pMesh->vertexList.size());
    for (unsigned int indice=0 ; indice < pMesh->vertexList.size(); indice++) {
        printf("Linha: %02d : p: %02d ( %05.3f ; %05.3f ; %05.3f )\n",linha, indice, pMesh->vertexList[indice].x, pMesh->vertexList[indice].y, pMesh->vertexList[indice].z);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("Normal Indice ----------( %03d )\n", pMesh->normalIndex.size());
    for (unsigned int indice=0 ; indice < pMesh->normalIndex.size(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, pMesh->normalIndex[indice], pMesh->normalIndex[indice + 1], pMesh->normalIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Normal Lista ---------( %03d )\n", pMesh->normalList.size());
    for (unsigned int indice=0 ; indice < pMesh->normalList.size(); indice++) {
        printf("Linha: %02d : p: %02d ( %05.3f ; %05.3f ; %05.3f )\n",linha, indice, pMesh->normalList[indice].x, pMesh->normalList[indice].y, pMesh->normalList[indice].z);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("Texture Indice ----------( %03d )\n", pMesh->textureIndex.size());
    for (unsigned int indice=0 ; indice < pMesh->textureIndex.size(); indice += 3) {
        printf("Linha: %02d : p: %02d ( %02d ; %02d ; %02d )\n",linha, indice, pMesh->textureIndex[indice], pMesh->textureIndex[indice + 1], pMesh->textureIndex[indice + 2]);
        linha++;
    }
    printf("\n");
    
    linha = 0;
    printf("-Texture Lista ---------( %03d )\n", pMesh->textureList.size());
    for (unsigned int indice=0 ; indice < pMesh->textureList.size(); indice++) {
        printf("Linha: %02d : p: %02d ( %05.3f ; %05.3f )\n",linha, indice, pMesh->textureList[indice].x, pMesh->textureList[indice].y);
        linha++;
    }
    printf("\n");  
}    
    
    
Mesh* createMeshParallelepiped(Node *_pParent, const std::string &_name, const glm::vec3 &_size, Texture* _pTexture, Material *_pMaterial) {

    //Mesh
    Mesh *pMesh = new Mesh(_pParent, _name);

    glm::vec3 sizeObj( _size.x / 2.0f , _size.y / 2.0f, _size.z / 2.0f);
    
    //VertexList
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x , -sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x ,  sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x ,  sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x , -sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x , -sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x ,  sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x ,  sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x , -sizeObj.y , -sizeObj.z ) );

    //VertexIndex
    int vertexIndexArray[] = { 01 , 02 , 03 ,
                                07 , 06 , 05 ,
                                04 , 05 , 01 ,
                                05 , 06 , 02 ,
                                06 , 07 , 03 ,
                                00 , 03 , 07 ,
                                00 , 01 , 03 ,
                                04 , 07 , 05 ,
                                00 , 04 , 01 ,
                                01 , 05 , 02 ,
                                02 , 06 , 03 ,
                                04 , 00 , 07 };
                            
    unsigned vertexIndexArraySize = sizeof(vertexIndexArray) / sizeof(int);
    pMesh->vertexIndex.insert(pMesh->vertexIndex.end(), &vertexIndexArray[0], &vertexIndexArray[vertexIndexArraySize]);
        
    //NormalList
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 , -1 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 ,  1 ));
    pMesh->normalList.push_back(glm::vec3(  1 , -0 ,  0 ));
    pMesh->normalList.push_back(glm::vec3( -0 , -1 ,  0 ));
    pMesh->normalList.push_back(glm::vec3( -1 ,  0 , -0 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  1 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 , -1 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 ,  1 ));
    pMesh->normalList.push_back(glm::vec3(  1 ,  0 , -0 ));
    pMesh->normalList.push_back(glm::vec3( -0 , -1 , -0 ));
    pMesh->normalList.push_back(glm::vec3( -1 ,  0 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  1 ,  0 ));

    //NormalIndex
    int normalIndexArray[] =  { 0,0,0,
                                1,1,1, 
                                2,2,2, 
                                3,3,3,
                                4,4,4,
                                5,5,5,
                                6,6,6,
                                7,7,7,
                                8,8,8,
                                9,9,9,
                                10,10,10,
                                11,11,11};

    unsigned normalIndexArraySize = sizeof(normalIndexArray) / sizeof(int);
    pMesh->normalIndex.insert(pMesh->normalIndex.end(), &normalIndexArray[0], &normalIndexArray[normalIndexArraySize]);
    
    if (_pTexture != nullptr) {
        //TextureList
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 1.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 1.000 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.000 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.000 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 1.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 1.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 1.000 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 1.000 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.667 ));
        pMesh->textureList.push_back( glm::vec2( 0.000 , 0.333 ));      
        
        int texturaIndexArray[] = { 0  , 1  , 2 ,
                                    3  , 4  , 5 ,
                                    6  , 7  , 8 ,
                                    9  , 10 , 11 ,
                                    12 , 13 , 14 ,
                                    15 , 16 , 17 ,
                                    18 , 19 , 20 ,
                                    21 , 22 , 23 ,
                                    24 , 25 , 26 ,
                                    27 , 28 , 29 ,
                                    30 , 31 , 32 ,
                                    33 , 34 , 35 };
        
        unsigned texturaIndexArraySize = sizeof(texturaIndexArray) / sizeof(int);
        pMesh->textureIndex.insert(pMesh->textureIndex.end(), &texturaIndexArray[0], &texturaIndexArray[texturaIndexArraySize]);  
        
        pMesh->setTexture(_pTexture);
    }
    
    if (_pMaterial != nullptr)
        pMesh->setMaterial(_pMaterial);

    return pMesh;
}

Mesh* createMeshParallelepiped2(Node *_pParent, const std::string &_name, const glm::vec3 &_size, Texture* _pTexture, Material *_pMaterial) {

    //Mesh
    Mesh *pMesh = new Mesh(_pParent, _name);

    glm::vec3 sizeObj( _size.x / 2.0f , _size.y / 2.0f, _size.z / 2.0f);
    
    //VertexList
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x ,  sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x , -sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x , -sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x ,  sizeObj.y , -sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x ,  sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3(  sizeObj.x , -sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x , -sizeObj.y ,  sizeObj.z ) );
    pMesh->vertexList.push_back( glm::vec3( -sizeObj.x ,  sizeObj.y ,  sizeObj.z ) );

    //VertexIndex
    int vertexIndexArray[] = {  00 , 01 , 03 ,
                                04 , 07 , 05 ,
                                00 , 04 , 01 ,
                                01 , 05 , 02 ,
                                02 , 06 , 03 ,
                                04 , 00 , 07 ,
                                01 , 02 , 03 ,
                                07 , 06 , 05 ,
                                04 , 05 , 01 ,
                                05 , 06 , 02 ,
                                06 , 07 , 03 ,
                                00 , 03 , 07 };
                            
    unsigned vertexIndexArraySize = sizeof(vertexIndexArray) / sizeof(int);
    pMesh->vertexIndex.insert(pMesh->vertexIndex.end(), &vertexIndexArray[0], &vertexIndexArray[vertexIndexArraySize]);
        
    //NormalList
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 , -1 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 ,  1 ));
    pMesh->normalList.push_back(glm::vec3(  1 ,  0 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 , -1 ,  0 ));
    pMesh->normalList.push_back(glm::vec3( -1 ,  0 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  1 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 , -1 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  0 ,  1 ));
    pMesh->normalList.push_back(glm::vec3(  1 ,  0 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 , -1 ,  0 ));
    pMesh->normalList.push_back(glm::vec3( -1 ,  0 ,  0 ));
    pMesh->normalList.push_back(glm::vec3(  0 ,  1 ,  0 ));

    //NormalIndex
    int normalIndexArray[] =  { 0,0,0,
                                1,1,1, 
                                2,2,2, 
                                3,3,3,
                                4,4,4,
                                5,5,5,
                                6,6,6,
                                7,7,7,
                                8,8,8,
                                9,9,9,
                                10,10,10,
                                11,11,11};

    unsigned normalIndexArraySize = sizeof(normalIndexArray) / sizeof(int);
    pMesh->normalIndex.insert(pMesh->normalIndex.end(), &normalIndexArray[0], &normalIndexArray[normalIndexArraySize]);
    
    if (_pTexture != nullptr) {
        //TextureList
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 1.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 1.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.000 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 1.000 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 1.000 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 1.000 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.250 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.000 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.000 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 1.000 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.666 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.500 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.333 ));
        pMesh->textureList.push_back( glm::vec2( 0.750 , 0.666 ));      
        
        int texturaIndexArray[] = { 0  , 1  , 2 ,
                                    3  , 4  , 5 ,
                                    6  , 7  , 8 ,
                                    9  , 10 , 11 ,
                                    12 , 13 , 14 ,
                                    15 , 16 , 17 ,
                                    18 , 19 , 20 ,
                                    21 , 22 , 23 ,
                                    24 , 25 , 26 ,
                                    27 , 28 , 29 ,
                                    30 , 31 , 32 ,
                                    33 , 34 , 35 };
        
        unsigned texturaIndexArraySize = sizeof(texturaIndexArray) / sizeof(int);
        pMesh->textureIndex.insert(pMesh->textureIndex.end(), &texturaIndexArray[0], &texturaIndexArray[texturaIndexArraySize]);  
        
        pMesh->setTexture(_pTexture);
    }
    
    if (_pMaterial != nullptr)
        pMesh->setMaterial(_pMaterial);

    return pMesh;
}
    
    

}


