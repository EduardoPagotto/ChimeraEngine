#include <map>
#include "VboIndexer.hpp"
// #include <glm/glm.hpp>
#include <string.h> // for memcmp

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2) {
	return fabs( v1 - v2 ) < 0.01f;
}

void conversorVBO(
    std::vector<unsigned int> & vertexIndex,
    std::vector<glm::vec3> & vertexList,
    std::vector<unsigned int> & normalIndex,
    std::vector<glm::vec3> & normalList,
    std::vector<unsigned int> & textureIndex,
    std::vector<glm::vec2> & textureList,
    std::vector<VertexData> &outData
) {
    
	unsigned l_numFaces = vertexIndex.size() / 3;
	unsigned int l_index = 0;
	unsigned int fa = 0;

	for (unsigned int face = 0; face < l_numFaces; face++) {
		fa = face * 3;
		for (unsigned int point = 0; point < 3; point++) {
			l_index = fa + point;
			outData.push_back({
				vertexList[vertexIndex[l_index]],
				normalList[normalIndex[l_index]],
				(textureIndex.size() > 0) ? textureList[textureIndex[l_index]] : glm::vec2(0.0,0.0) }
			);
		}
	}
    
}

bool getSimilarVertexIndex( VertexData &in_vertex, std::vector<VertexData> &out_vertex, unsigned int & result) {
	// Percorrer todos os vertex ja existentes na lista
	for (unsigned int i = 0; i < out_vertex.size(); i++) {

		if (is_near(in_vertex.position.x, out_vertex[i].position.x) &&
			is_near(in_vertex.position.y, out_vertex[i].position.y) &&
			is_near(in_vertex.position.z, out_vertex[i].position.z) &&
			is_near(in_vertex.normal.x, out_vertex[i].normal.x) &&
			is_near(in_vertex.normal.y, out_vertex[i].normal.y) &&
			is_near(in_vertex.normal.z, out_vertex[i].normal.z) &&
			is_near(in_vertex.texture.x, out_vertex[i].texture.x) &&
			is_near(in_vertex.texture.y, out_vertex[i].texture.y))
		{
			result = i;
			return true;
		}
	}
	return false;
}

void indexVBO_slow(std::vector<VertexData> &inData, std::vector<VertexData> &outData, std::vector<unsigned int> & out_indices) {
	
	//percorrer todos os vertices
	for (unsigned int i = 0; i< inData.size(); i++) {

		//Procura por similar
		unsigned int index;
		if (getSimilarVertexIndex(inData[i], outData, index) == true) {

			// se entrotar usar apenas o indice
			out_indices.push_back(index);

		} else {

			//se nao adiciona a lista de novo vertex
			outData.push_back(inData[i]);
			out_indices.push_back((unsigned int)outData.size() - 1);
		}
	}
}
