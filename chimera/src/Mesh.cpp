#include "Mesh.h"
#include "ChimeraUtils.h"
#include "NodeVisitor.h"
#include <iterator>

#include "MeshUtil.h"

namespace Chimera {
    
Mesh::Mesh (Node *_parent, std::string _name ) : Draw (_parent, EntityKind::MESH, _name ) {
    VertexVBOID = 0;
    IndexVBOID = 0;
    VAO = 0;
}

Mesh::Mesh ( const Mesh &_cpy ) : Draw ( _cpy ) {
        
  vertexIndex.reserve(_cpy.vertexIndex.size());
  copy(_cpy.vertexIndex.begin(), _cpy.vertexIndex.end(), back_inserter(vertexIndex));
  copy(_cpy.vertexList.begin(), _cpy.vertexList.end(), back_inserter(vertexList));
  
  normalIndex.reserve(_cpy.normalIndex.size());
  copy(_cpy.normalIndex.begin(), _cpy.normalIndex.end(), back_inserter(normalIndex));
  copy(_cpy.normalList.begin(), _cpy.normalList.end(), back_inserter(normalList));
  
  textureIndex.reserve(_cpy.textureIndex.size());
  copy(_cpy.textureIndex.begin(), _cpy.textureIndex.end(), back_inserter(textureIndex));
  copy(_cpy.textureList.begin(), _cpy.textureList.end(), back_inserter(textureList));

}

Mesh::~Mesh() {

    vertexIndex.clear();
    vertexList.clear();
    normalIndex.clear();
    normalList.clear();
    textureIndex.clear();
    textureList.clear();
   
	glDeleteBuffers(1, &VertexVBOID);
	glDeleteBuffers(1, &IndexVBOID);
}

void Mesh::init() {
    
    if (pTexture != nullptr) {
        pTexture->init();
        pState->setEnableTexture(pTexture, true);
    }
    
    if (pMaterial == nullptr) {
        pMaterial = new Material("DefaultMat");
        pMaterial->createDefaultEffect();
    }

    pMaterial->init();
    pState->setEnableMaterial(pMaterial, true);
    
     //Chimera::debugDados(this);//FIXME: vai dar pau no windows
    
    //Ajuste de textura do imageSDL invertendo valor de V
    int tamanho = textureIndex.size();
    for (int indice = 0; indice < tamanho; indice++) {
        int posTex = textureIndex[indice];
        textureList[posTex].y = 1 - textureList[posTex].y;
    }
    
    if (shader.getIdProgram() > 0 )
        setVertexBuffer();
    else 
        setVertexBufferOnoShade();
}

void Mesh::accept(NodeVisitor * v) {
    v->visit(this);
}

glm::vec3 Mesh::getSizeBox() {
    
    glm::vec3 retorno ( 0.0f, 0.0f, 0.0f );
    glm::vec3 l_max ( 0.0f, 0.0f, 0.0f );
    glm::vec3 l_min ( 0.0f, 0.0f, 0.0f );

    for (unsigned int indice = 0; indice < vertexList.size(); indice++ ) {
        
        if ( l_max.x < vertexList[indice].x )
            l_max.x = vertexList[indice].x;
      
        if ( l_max.y < vertexList[indice].y )
            l_max.y = vertexList[indice].y;

        if ( l_max.z < vertexList[indice].z )
            l_max.z = vertexList[indice].z;

        if ( l_min.x > vertexList[indice].x )
            l_min.x = vertexList[indice].x;

        if ( l_min.y > vertexList[indice].y )
            l_min.y = vertexList[indice].y;

        if ( l_min.z > vertexList[indice].z )
            l_min.z = vertexList[indice].z;
    }

    return glm::vec3(( glm::abs ( l_max.x ) + glm::abs ( l_min.x ) ) / 2,
                     ( glm::abs ( l_max.y ) + glm::abs ( l_min.y ) ) / 2,
                     ( glm::abs ( l_max.z ) + glm::abs ( l_min.z ) ) / 2 );
}

//https://cognitivewaves.wordpress.com/opengl-vbo-shader-vao/ <- a resposta aqui!!!!! matrix de model e projecao
void Mesh::renderExecute(bool _texture) {

    if (shader.getIdProgram() > 0)
        renderVertexBuffer(_texture);
    else
        renderVertexBufferOnoShade(_texture);
}

void Mesh::setVertexBufferOnoShade() {
    
    std::vector<VertexData> vertexDataIn;
    conversorVBO(vertexIndex, vertexList, normalIndex, normalList, textureIndex, textureList, pMaterial->getDiffuse() ,vertexDataIn);
    indexVBO_slow(vertexDataIn, vertexData, indexIBO);

    printf("Nome: %s \n", getName().c_str());
    printf("-VBO Indice ---------( %03d )\n", indexIBO.size());
    printf("-VBO Data -----------( %03d )\n", vertexData.size());
    printf("\n");

    //FBO sem o uso de VAO
	int sizeOfStruct = sizeof(struct VertexData);
    unsigned int sizeBufferVertex = vertexData.size() * sizeOfStruct;
    glGenBuffers(1, &VertexVBOID); //Gera o buffer de dados do Vertex
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID); //vincula o buffer de dados
    glBufferData(GL_ARRAY_BUFFER, sizeBufferVertex, &vertexData[0], GL_STATIC_DRAW); //passa o buffer 

    unsigned int sizeBufferIndex = indexIBO.size() * sizeof(unsigned int);
    glGenBuffers(1, &IndexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &indexIBO[0], GL_STATIC_DRAW);
    
}

void Mesh::renderVertexBufferOnoShade(bool _texture) {
    
    if (_texture == true) {

        if (pState->getTexture() != nullptr)
            pState->appyTexture();
        else
            glBindTexture(GL_TEXTURE_2D, 0);

        pState->appyMaterial();
        
		//Usando Apenas FBO
        //vincula VBO
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
        //points
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(0));

        //Normal
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(12));

        //Texture
		if (pState->getTexture() != nullptr) {
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(24));
		}

		//Color
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(32)); 
		
        //vincula indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

        //draw
        glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        //desabilita client stats
        glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

        if (pState->getTexture() != nullptr)
            glDisableClientState(GL_TEXTURE0);
        
        glDisableClientState(GL_COLOR_ARRAY);
         
    } else {

        pState->appyMaterial();

        //vincula VertexData
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
        //points
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
		//normal														       //Normal
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(12));   //The starting point of normals, 12 bytes away
        //color                                                               //Color
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, sizeof(VertexData), BUFFER_OFFSET(32));   //The starting point of the VBO, for the vertices                                                                            
        //vincula indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

        //draw
        glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        //desabilita client stats
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

void Mesh::renderVertexBuffer ( bool _texture ) {

        if (_texture == true) {

        if (pState->getTexture() != nullptr)
            pState->appyTexture();
        else
            glBindTexture(GL_TEXTURE_2D, 0);

        pState->appyMaterial();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        glBindVertexArray(0);

    }
    else {
		//TODO: como fazer isso??
    }
}

void Mesh::setVertexBuffer()
{
	std::vector<VertexData> vertexDataIn;
	conversorVBO(vertexIndex, vertexList, normalIndex, normalList, textureIndex, textureList, pMaterial->getDiffuse(), vertexDataIn);
	indexVBO_slow(vertexDataIn, vertexData, indexIBO);

	printf("Nome: %s \n", getName().c_str());
	printf("-VBO Indice ---------( %03d )\n", indexIBO.size());
	printf("-VBO Data -----------( %03d )\n", vertexData.size());
	printf("\n");

	unsigned int sizeBufferVertex = vertexData.size() * sizeof(VertexData);
	unsigned int sizeBufferIndex = indexIBO.size() * sizeof(unsigned int);

	//Buffer de vertice
	glGenBuffers(1, &VertexVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeBufferVertex, &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Buffer de indice
	glGenBuffers(1, &IndexVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &indexIBO[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//cria o VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vincula VBO
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);

	//Vertice
	GLuint positionID = glGetAttribLocation(shader.getIdProgram(), "vertex");
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(positionID);
	
	//Normal
	GLuint normalID = glGetAttribLocation(shader.getIdProgram(), "normal");
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),  BUFFER_OFFSET(12));
	glEnableVertexAttribArray(normalID);

	//Texture
	GLuint uvID = glGetAttribLocation(shader.getIdProgram(), "uv1");
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),  BUFFER_OFFSET(24));
	glEnableVertexAttribArray(uvID);

    //Color
    GLuint colorID = glGetAttribLocation(shader.getIdProgram(), "color");
    glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(32));
    glEnableVertexAttribArray(colorID);    
    
	//vincula IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

	//limpa dados
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(normalID);
	glDisableVertexAttribArray(uvID);
    glDisableVertexAttribArray(colorID);

	//  Model::Draw(ICamera camera) {
	//     GLuint matrixID = glGetUniformLocation(programID, "mvp");
	//     GLuint positionID = glGetAttribLocation(programID, "position_modelspace");
	//     GLuint uvID = glGetAttribLocation(programID, "uv");
	//     GLuint normalID = glGetAttribLocation(programID, "normal_modelspace");
	//     GLuint tangentID = glGetAttribLocation(programID, "tangent_modelspace");
	//     GLuint bitangentID = glGetAttribLocation(programID, "bitangent_modelspace");
	// 
	//     glm::mat4 projection = camera->GetProjectionMatrix(); 
	//     glm::mat4 view = camera->GetViewMatrix();
	//     glm::mat4 model = glm::mat4(1.0f);
	//     glm::mat4 mvp = projection * view * model;
	// 
	//     glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	// 
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
                
                for (unsigned int indice=0; indice < lista.size(); indice += 3) 
                    vertexList.push_back( glm::vec3(lista[indice],lista[indice+1],lista[indice+2]) );
                   
            } else if ( strstr ( l_id, ( char* ) "-normals" ) != nullptr ) {
                
                //carrega todos os vetores normal
                std::vector<float> lista;
                getSource ( l_nSource, lista );

                for (unsigned int indice=0; indice < lista.size(); indice += 3) 
                    normalList.push_back( glm::vec3(lista[indice],lista[indice+1],lista[indice+2]));
                  
            } else if ( strstr ( l_id, ( char* ) "-map-0" ) != nullptr ) {
                
                //carrega vetor posicao textura
                std::vector<float> lista;
                getSource ( l_nSource, lista );
                for (unsigned int indice=0; indice < lista.size(); indice += 2)
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
    printf("-Vertex  Indice / Lista ------ ( %03d / %03d )\n", vertexIndex.size(), vertexList.size());
	printf("-Normal  Indice / Lista ------ ( %03d / %03d )\n", normalIndex.size(), normalList.size());
    printf("-Texture Indice / Lista ------ ( %03d / %03d )\n", textureIndex.size(), textureList.size());
    printf("\n");
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
