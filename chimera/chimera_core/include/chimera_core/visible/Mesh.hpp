#pragma once
#include "chimera_space/Triangle.hpp"
#include "chimera_space/Vertex.hpp"
#include <glm/glm.hpp>
#include <list>
#include <memory>

namespace ce {

    struct Mesh {
        Mesh() = default;
        std::vector<VertexData> vertex;
        TrisIndex iFace;
    };

    enum class MeshType { SIMPLE = 0, ARRAY = 1, BSTREE = 2 };

    MeshType get_mesh_type_from_string(const std::string& text);
    void mesh_to_triangle(Mesh& m, std::list<std::shared_ptr<Triangle>>& v_tris);
    void mesh_reindex(Mesh& in_data, Mesh& out_data);
    void mesh_debug(const Mesh& m, bool show_all);
    void mesh_serialize(Mesh& in_data, Mesh& out_data);
    void idx_simplifie_vec3(std::vector<glm::vec3>& in, std::vector<glm::vec3>& out, std::vector<uint32_t>& idx_in,
                            std::vector<uint32_t>& idx_out);
    void idx_simplifie_vec2(std::vector<glm::vec2>& in, std::vector<glm::vec2>& out, std::vector<uint32_t>& idx_in,
                            std::vector<uint32_t>& idx_out);

    std::tuple<glm::vec3, glm::vec3, glm::vec3> vertex_boundaries(std::vector<VertexData>& v_array);
    std::tuple<glm::vec3, glm::vec3, glm::vec3> vertex_indexed_boundaries(std::vector<VertexData>& v_array,
                                                                          TrisIndex& tris);

} // namespace ce
