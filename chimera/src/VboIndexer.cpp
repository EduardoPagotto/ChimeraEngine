#include <map>
#include "VboIndexer.hpp"
// #include <glm/glm.hpp>
#include <string.h> // for memcmp

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2){
	return fabs( v1-v2 ) < 0.01f;
}

// // Searches through all already-exported vertices
// // for a similar one.
// // Similar = same position + same UVs + same normal
// bool getSimilarVertexIndex( 
// 	glm::vec3 & in_vertex, 
// 	glm::vec2 & in_uv, 
// 	glm::vec3 & in_normal, 
// 	std::vector<glm::vec3> & out_vertices,
// 	std::vector<glm::vec2> & out_uvs,
// 	std::vector<glm::vec3> & out_normals,
// 	unsigned int & result
// ){
// 	// Lame linear search
// 	for ( unsigned int i=0; i<out_vertices.size(); i++ ){
// 		if (
// 			is_near( in_vertex.x , out_vertices[i].x ) &&
// 			is_near( in_vertex.y , out_vertices[i].y ) &&
// 			is_near( in_vertex.z , out_vertices[i].z ) &&
// 			is_near( in_uv.x     , out_uvs     [i].x ) &&
// 			is_near( in_uv.y     , out_uvs     [i].y ) &&
// 			is_near( in_normal.x , out_normals [i].x ) &&
// 			is_near( in_normal.y , out_normals [i].y ) &&
// 			is_near( in_normal.z , out_normals [i].z )
// 		){
// 			result = i;
// 			return true;
// 		}
// 	}
// 	// No other vertex could be used instead.
// 	// Looks like we'll have to add it to the VBO.
// 	return false;
// }
// 
// bool getSimilarVertexIndexNoTex( 
//     glm::vec3 & in_vertex, 
//     glm::vec3 & in_normal, 
//     std::vector<glm::vec3> & out_vertices,
//     std::vector<glm::vec3> & out_normals,
//     unsigned int & result
// ){
//     // Lame linear search
//     for ( unsigned int i=0; i<out_vertices.size(); i++ ){
//         if (
//             is_near( in_vertex.x , out_vertices[i].x ) &&
//             is_near( in_vertex.y , out_vertices[i].y ) &&
//             is_near( in_vertex.z , out_vertices[i].z ) &&
//             is_near( in_normal.x , out_normals [i].x ) &&
//             is_near( in_normal.y , out_normals [i].y ) &&
//             is_near( in_normal.z , out_normals [i].z )
//         ){
//             result = i;
//             return true;
//         }
//     }
//     // No other vertex could be used instead.
//     // Looks like we'll have to add it to the VBO.
//     return false;
// }
// 
// void indexVBO_slow(
// 	std::vector<glm::vec3> & in_vertices,
// 	std::vector<glm::vec2> & in_uvs,
// 	std::vector<glm::vec3> & in_normals,
// 
// 	std::vector<unsigned int> & out_indices,
// 	std::vector<glm::vec3> & out_vertices,
// 	std::vector<glm::vec2> & out_uvs,
// 	std::vector<glm::vec3> & out_normals
// ){
// 	// For each input vertex
// 	for ( unsigned int i=0; i<in_vertices.size(); i++ ){
// 
// 		// Try to find a similar vertex in out_XXXX
// 		unsigned int index;
// 		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);
// 
// 		if ( found ){ // A similar vertex is already in the VBO, use it instead !
// 			out_indices.push_back( index );
// 		}else{ // If not, it needs to be added in the output data.
// 			out_vertices.push_back( in_vertices[i]);
// 			out_uvs     .push_back( in_uvs[i]);
// 			out_normals .push_back( in_normals[i]);
// 			out_indices .push_back( (unsigned int)out_vertices.size() - 1 );
// 		}
// 	}
// }
// 
// void indexVBO_slowNoTex(
//     std::vector<glm::vec3> & in_vertices,
//     std::vector<glm::vec3> & in_normals,
// 
//     std::vector<unsigned int> & out_indices,
//     std::vector<glm::vec3> & out_vertices,
//     std::vector<glm::vec3> & out_normals
// ){
//     // For each input vertex
//     for ( unsigned int i=0; i<in_vertices.size(); i++ ){
// 
//         // Try to find a similar vertex in out_XXXX
//         unsigned int index;
//         bool found = getSimilarVertexIndexNoTex(in_vertices[i], in_normals[i], out_vertices, out_normals, index);
// 
//         if ( found ){ // A similar vertex is already in the VBO, use it instead !
//             out_indices.push_back( index );
//         }else{ // If not, it needs to be added in the output data.
//             out_vertices.push_back( in_vertices[i]);
//             out_normals .push_back( in_normals[i]);
//             out_indices .push_back( (unsigned int)out_vertices.size() - 1 );
//         }
//     }
// }
/*
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
) {

	unsigned l_numFaces = vertexIndex.size() / 3;
	unsigned int l_index = 0;
	unsigned int fa = 0;
	for (unsigned int face = 0; face < l_numFaces; face++) {
		fa = face * 3;

		for (unsigned int point = 0; point < 3; point++) {
			l_index = fa + point;

			out_indices.push_back(l_index);

			unsigned int posicao = vertexIndex[l_index];
			unsigned int posNormal = normalIndex[l_index];

			out_normals.push_back(normalList[posNormal]);

			if (textureIndex.size() > 0)
				out_uvs.push_back(textureList[textureIndex[l_index]]);

			out_vertices.push_back(vertexList[posicao]);
		}
	}
}*/

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
         outData.push_back( { 
             vertexList[vertexIndex[l_index]],
             normalList[normalIndex[l_index]], 
             (textureIndex.size() > 0) ? textureList[textureIndex[l_index]] : glm::vec2(0.0,0.0) } 
         );
     }
 }
    
}

bool getSimilarVertexIndex2( VertexData &in_vertex, std::vector<VertexData> &out_vertex, unsigned int & result) {
 // Lame linear search
 for (unsigned int i = 0; i < out_vertex.size(); i++) {
     
     if (is_near(in_vertex.position.x , out_vertex[i].position.x ) &&
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
 // No other vertex could be used instead.
 // Looks like we'll have to add it to the VBO.
 return false;
}

void indexVBO_slow(
 std::vector<VertexData> &inData,
 std::vector<VertexData> &outData,
 std::vector<unsigned int> & out_indices) {
 // For each input vertex
 for (unsigned int i = 0; i< inData.size(); i++) {

     // Try to find a similar vertex in out_XXXX
     unsigned int index;
     bool found = getSimilarVertexIndex2(inData[i], outData, index);

     if (found) { // A similar vertex is already in the VBO, use it instead !
         out_indices.push_back(index);
     } else { // If not, it needs to be added in the output data.

         outData.push_back(inData[i]);
         out_indices.push_back((unsigned int)outData.size() - 1);
     }
 }
}

// void indexVBO_slow(
//     std::vector<VertexData> &inData,
//     std::vector<unsigned int> & out_indices,
//     std::vector<glm::vec3> & out_vertices,
//     std::vector<glm::vec2> & out_uvs,
//     std::vector<glm::vec3> & out_normals
// ) {
//     // For each input vertex
//     for (unsigned int i = 0; i< inData.size(); i++) {
// 
//         // Try to find a similar vertex in out_XXXX
//         unsigned int index;
//         bool found = getSimilarVertexIndex2(inData[i],inData, index);
// 
//         if (found) { // A similar vertex is already in the VBO, use it instead !
//             out_indices.push_back(index);
//         }
//         else { // If not, it needs to be added in the output data.
//             out_vertices.push_back(inData[i].position);
//             out_uvs.push_back(inData[i].texture );
//             out_normals.push_back(inData[i].normal);
//             out_indices.push_back((unsigned int)out_vertices.size() - 1);
//         }
//     }
// }

//struct PackedVertex{
//	glm::vec3 position;
//	glm::vec2 uv;
//	glm::vec3 normal;
//	bool operator<(const PackedVertex that) const{
//		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
//	};
//};
//
//bool getSimilarVertexIndex_fast( 
//	PackedVertex & packed, 
//	std::map<PackedVertex,unsigned int> & VertexToOutIndex,
//	unsigned int & result
//){
//	std::map<PackedVertex,unsigned int>::iterator it = VertexToOutIndex.find(packed);
//	if ( it == VertexToOutIndex.end() ){
//		return false;
//	}else{
//		result = it->second;
//		return true;
//	}
//}
//
//void indexVBO(
//	std::vector<glm::vec3> & in_vertices,
//	std::vector<glm::vec2> & in_uvs,
//	std::vector<glm::vec3> & in_normals,
//
//	std::vector<unsigned int> & out_indices,
//	std::vector<glm::vec3> & out_vertices,
//	std::vector<glm::vec2> & out_uvs,
//	std::vector<glm::vec3> & out_normals
//){
//	std::map<PackedVertex,unsigned int> VertexToOutIndex;
//
//	// For each input vertex
//	for ( unsigned int i=0; i<in_vertices.size(); i++ ){
//
//		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
//		
//
//		// Try to find a similar vertex in out_XXXX
//		unsigned int index;
//		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);
//
//		if ( found ){ // A similar vertex is already in the VBO, use it instead !
//			out_indices.push_back( index );
//		}else{ // If not, it needs to be added in the output data.
//			out_vertices.push_back( in_vertices[i]);
//			out_uvs     .push_back( in_uvs[i]);
//			out_normals .push_back( in_normals[i]);
//			unsigned int newindex = (unsigned int)out_vertices.size() - 1;
//			out_indices .push_back( newindex );
//			VertexToOutIndex[ packed ] = newindex;
//		}
//	}
//}
//
//void indexVBOnoTexture(
//    std::vector<glm::vec3> & in_vertices,
//    std::vector<glm::vec3> & in_normals,
//
//    std::vector<unsigned int> & out_indices,
//    std::vector<glm::vec3> & out_vertices,
//    std::vector<glm::vec3> & out_normals
//){
//    std::map<PackedVertex,unsigned int> VertexToOutIndex;
//
//    // For each input vertex
//    for ( unsigned int i=0; i<in_vertices.size(); i++ ){
//
//        PackedVertex packed = {in_vertices[i], glm::vec2(0.0,0.0), in_normals[i]};
//        
//
//        // Try to find a similar vertex in out_XXXX
//        unsigned int index;
//        bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);
//
//        if ( found ){ // A similar vertex is already in the VBO, use it instead !
//            out_indices.push_back( index );
//        }else{ // If not, it needs to be added in the output data.
//            out_vertices.push_back( in_vertices[i]);
//            out_normals .push_back( in_normals[i]);
//            unsigned int newindex = (unsigned int)out_vertices.size() - 1;
//            out_indices .push_back( newindex );
//            VertexToOutIndex[ packed ] = newindex;
//        }
//    }
//}
//
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
//){
//	// For each input vertex
//	for ( unsigned int i=0; i<in_vertices.size(); i++ ){
//
//		// Try to find a similar vertex in out_XXXX
//		unsigned int index;
//		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);
//
//		if ( found ){ // A similar vertex is already in the VBO, use it instead !
//			out_indices.push_back( index );
//
//			// Average the tangents and the bitangents
//			out_tangents[index] += in_tangents[i];
//			out_bitangents[index] += in_bitangents[i];
//		}else{ // If not, it needs to be added in the output data.
//			out_vertices.push_back( in_vertices[i]);
//			out_uvs     .push_back( in_uvs[i]);
//			out_normals .push_back( in_normals[i]);
//			out_tangents .push_back( in_tangents[i]);
//			out_bitangents .push_back( in_bitangents[i]);
//			out_indices .push_back( (unsigned int)out_vertices.size() - 1 );
//		}
//	}
//}



//void conversorVBO(
//	std::vector<unsigned int> & vertexIndex,
//	std::vector<glm::vec3> & vertexList,
//	std::vector<unsigned int> & normalIndex,
//	std::vector<glm::vec3> & normalList,
//	std::vector<unsigned int> & textureIndex,
//	std::vector<glm::vec2> & textureList,
//
//	std::vector<Vertex> &out
//) {
//	unsigned l_numFaces = vertexIndex.size() / 3;
//	unsigned int l_index = 0;
//	unsigned int fa = 0;
//	for (unsigned int face = 0; face < l_numFaces; face++) {
//		fa = face * 3;
//		for (unsigned int point = 0; point < 3; point++) {
//			l_index = fa + point;
//			out.push_back( { vertexList[vertexIndex[l_index]], normalList[normalIndex[l_index]], (textureIndex.size() > 0) ? textureList[textureIndex[l_index]] : glm::vec2(0.0,0.0) } );
//		}
//	}
//}

