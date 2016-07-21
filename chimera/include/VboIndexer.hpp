#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP

#include <vector>
#include <glm/glm.hpp>

void indexVBO_slow(
    std::vector<glm::vec3> & in_vertices,
    std::vector<glm::vec2> & in_uvs,
    std::vector<glm::vec3> & in_normals,

    std::vector<unsigned int> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
);

void indexVBO_slowNoTex(
    std::vector<glm::vec3> & in_vertices,
    std::vector<glm::vec3> & in_normals,

    std::vector<unsigned int> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec3> & out_normals
);

void conversorVBO(
	std::vector<unsigned int> & vertexIndex,
	std::vector<glm::vec3> & vertexList,
	std::vector<unsigned int> & normalIndex,
	std::vector<glm::vec3> & normalList,
	std::vector<unsigned int> & textureIndex,
	std::vector<glm::vec2> & textureList,

	std::vector<unsigned int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

//void indexVBO(
//	std::vector<glm::vec3> & in_vertices,
//	std::vector<glm::vec2> & in_uvs,
//	std::vector<glm::vec3> & in_normals,
//
//	std::vector<unsigned int> & out_indices,
//	std::vector<glm::vec3> & out_vertices,
//	std::vector<glm::vec2> & out_uvs,
//	std::vector<glm::vec3> & out_normals
//);


//void indexVBO_TBN(
//	std::vector<glm::vec3> & in_vertices,
//	std::vector<glm::vec2> & in_uvs,
//	std::vector<glm::vec3> & in_normals,
//	std::vector<glm::vec3> & in_tangents,
//	std::vector<glm::vec3> & in_bitangents,
//
//	std::vector<unsigned int> & out_indices,
//	std::vector<glm::vec3> & out_vertices,
//	std::vector<glm::vec2> & out_uvs,
//	std::vector<glm::vec3> & out_normals,
//	std::vector<glm::vec3> & out_tangents,
//	std::vector<glm::vec3> & out_bitangents
//);

//void indexVBOnoTexture(
//    std::vector<glm::vec3> & in_vertices,
//    std::vector<glm::vec3> & in_normals,
//
//    std::vector<unsigned int> & out_indices,
//    std::vector<glm::vec3> & out_vertices,
//    std::vector<glm::vec3> & out_normals
//);

//struct Vertex {
//	glm::vec3 position;
//	glm::vec3 normal;
//	glm::vec2 texture;
//};
//
//void conversorVBO(
//	std::vector<unsigned int> & vertexIndex,
//	std::vector<glm::vec3> & vertexList,
//	std::vector<unsigned int> & normalIndex,
//	std::vector<glm::vec3> & normalList,
//	std::vector<unsigned int> & textureIndex,
//	std::vector<glm::vec2> & textureList,
//
//	std::vector<Vertex> &out
//);

#endif