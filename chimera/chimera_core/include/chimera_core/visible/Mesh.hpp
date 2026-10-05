#pragma once
#include "chimera_space/Triangle.hpp"
#include <glm/glm.hpp>
#include <list>
#include <memory>

namespace ce {

    // TODO: subistituir no futuro
    // // Auxiliar triângulo na CPU para ler indices
    // glm::ivec3 getTriangle(size_t faceIndex, uint32_t* indices) {
    //     const uint32_t a = faceIndex * 3;
    //     return {indices[a], indices[a + 1], indices[a + 2]};
    // }

    // // Auxiliar triângulo na CPU para gravar indices
    // void setTriangle(glm::ivec3 face, size_t faceIndex, uint32_t* indices) {
    //     const uint32_t a = faceIndex * 3;
    //     indices[a] = face.x;
    //     indices[a + 1] = face.y;
    //     indices[a + 2] = face.z;
    // }
    //
    // struct Vertex {
    //     glm::vec3 point{0.0};
    //     glm::vec3 normal{0.0};
    //     glm::vec2 uv{0.0};
    // };

    // struct SubMesh {
    //     AABB aabb;
    //     std::vector<uint32_t> indexes;
    //     std::optional<uint32_t> mIndexes;
    // };

    // struct CompleteMesh {
    //     std::vector<Vertex> vertex;
    //     std::vector<SubMesh> subs; // Each part represents a glTF primitive
    // };

    struct VertexData {
        glm::vec3 point;  // 3 * 4 = 12 ( 0 - 11)
        glm::vec3 normal; // 3 * 4 = 12 (12 - 23)
        glm::vec2 uv;     // 2 * 4 = 08 (24 - 31)
    };

    struct Mesh {
        Mesh() = default;
        std::vector<VertexData> vertex;
        TrisIndex iFace;
    };

    enum class MeshType { SIMPLE = 0, ARRAY = 1, BSTREE = 2 };

    MeshType getMeshTypeFromString(const std::string& text);
    void meshToTriangle(Mesh& m, std::list<std::shared_ptr<Triangle>>& v_tris);
    void meshReindex(Mesh& in_data, Mesh& out_data);
    void meshDebug(const Mesh& m, bool show_all);
    void meshSerialize(Mesh& in_data, Mesh& out_data);
    void idxSimplifieVec3(std::vector<glm::vec3>& in, std::vector<glm::vec3>& out, std::vector<uint32_t>& idx_in,
                          std::vector<uint32_t>& idx_out);
    void idxSimplifieVec2(std::vector<glm::vec2>& in, std::vector<glm::vec2>& out, std::vector<uint32_t>& idx_in,
                          std::vector<uint32_t>& idx_out);

    std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexBoundaries(std::vector<VertexData>& v_array);
    std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexIndexedBoundaries(std::vector<VertexData>& v_array,
                                                                        TrisIndex& tris);

} // namespace ce
