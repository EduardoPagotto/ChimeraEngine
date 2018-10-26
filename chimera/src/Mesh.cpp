#include "Mesh.h"
#include "NodeVisitor.h"
#include <iterator>

#include "MeshUtil.h"
#include "Singleton.h"

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders

namespace Chimera {

Mesh::Mesh (Node *_parent, std::string _name ) : Draw (_parent, EntityKind::MESH, _name ) {
    VertexVBOID = 0;
    IndexVBOID = 0;
    VAO = 0;
    log = spdlog::get("chimera");
    log->debug("Constructor Mesh:{}", _name);
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


    log = spdlog::get("chimera");
    log->debug("Constructor Copy Mesh {}", _cpy.getName());
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

    log->debug("Destructor Mesh:{}", getName());
}

void Mesh::init() {

	if (material == nullptr)
		material = new Material("DefaultMat-" + std::to_string(getSerial()));

	material->init();

    //Ajuste de textura do imageSDL invertendo valor de V
    for (int indice = 0; indice < textureIndex.size(); indice++) {
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



void Mesh::render () {

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
}

void Mesh::setVertexBuffer()
{
	std::vector<VertexData> vertexDataIn;
	conversorVBO(vertexIndex, vertexList, normalIndex, normalList, textureIndex, textureList, vertexDataIn);
	indexVBO_slow(vertexDataIn, vertexData, indexIBO);

    log->debug("VBO Nome: {0} Indice: {1} Data: {2}", getName().c_str(), indexIBO.size(), vertexData.size());

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
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
