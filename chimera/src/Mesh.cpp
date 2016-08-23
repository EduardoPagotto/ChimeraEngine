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

	if (pState->getMaterial() == nullptr) {
		Material* pMat = new Material("DefaultMat-" + std::to_string(getSerial()));
		pState->setMaterial(pMat);
	}

	pState->init();

    //Ajuste de textura do imageSDL invertendo valor de V
    int tamanho = textureIndex.size();
    for (int indice = 0; indice < tamanho; indice++) {
        int posTex = textureIndex[indice];
        textureList[posTex].y = 1 - textureList[posTex].y;
    }

    setVertexBuffer();
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

void Mesh::renderExecute ( bool _texture ) {

    pState->appyMaterial(&shader);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
}

void Mesh::setVertexBuffer()
{
	std::vector<VertexData> vertexDataIn;
	conversorVBO(vertexIndex, vertexList, normalIndex, normalList, textureIndex, textureList, vertexDataIn);
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
	GLuint positionID = 0;//glGetAttribLocation(shader.getIdProgram(), "position");
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(positionID);

	//Normal
	GLuint normalID = 1;//glGetAttribLocation(shader.getIdProgram(), "normal");
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),  BUFFER_OFFSET(12));
	glEnableVertexAttribArray(normalID);

	//Texture
	GLuint uvID = 2;//glGetAttribLocation(shader.getIdProgram(), "uv1");
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),  BUFFER_OFFSET(24));
	glEnableVertexAttribArray(uvID);

	//vincula IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

	//limpa dados
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(normalID);
	glDisableVertexAttribArray(uvID);
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
