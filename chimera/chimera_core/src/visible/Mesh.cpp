#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/space.hpp"
#include <SDL3/SDL.h>

namespace ce {

    MeshType get_mesh_type_from_string(const std::string& text) {
        if (text == "ARRAY") {
            return MeshType::ARRAY;
        } else if (text == "BSPTREE") {
            return MeshType::BSTREE;
        }

        return MeshType::SIMPLE;
    }

    void mesh_to_triangle(Mesh& m, std::list<std::shared_ptr<Triangle>>& v_tris) {
        for (uint32_t i = 0; i < m.iFace.size(); i++) {
            const glm::vec3 acc =
                m.vertex[m.iFace[i].x].normal + m.vertex[m.iFace[i].y].normal + m.vertex[m.iFace[i].z].normal;
            v_tris.push_back(std::make_shared<Triangle>(m.iFace[i], glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3), false));
        }
    }

    void idx_simplifie_vec2(std::vector<glm::vec2>& in, std::vector<glm::vec2>& out, std::vector<uint32_t>& idx_in,
                            std::vector<uint32_t>& idx_out) {

        // percorrer todos os vertices
        bool find;
        for (uint32_t i = 0; i < idx_in.size(); i++) {

            find = false;
            const glm::vec2& p1 = in[idx_in[i]];
            for (uint32_t j = 0; j < idx_out.size(); j++) {
                if (const glm::vec2& p2 = out[j]; is_near_v2(p1, p2)) { // Procura por similar
                    idx_out.push_back(j);
                    find = true;
                    break;
                }
            }

            if (find)
                continue; // FIXME: ver o std C17 para continue com label igual a rust e golang

            // se diferente adiciona ponto e cria novo indice
            out.push_back(p1);
            idx_out.push_back(out.size() - 1);
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Simplify2 In: %04lu out: %04lu faces: %04lu ", in.size(), out.size(),
                     idx_out.size() / 3);
    }

    void idx_simplifie_vec3(std::vector<glm::vec3>& in, std::vector<glm::vec3>& out, std::vector<uint32_t>& idx_in,
                            std::vector<uint32_t>& idx_out) {

        // percorrer todos os vertices
        bool find;
        for (uint32_t i = 0; i < idx_in.size(); i++) {

            // Procura por similar
            find = false;
            const glm::vec3& p1 = in[idx_in[i]];

            for (uint32_t j = 0; j < idx_out.size(); j++) {
                if (const glm::vec3& p2 = out[j]; is_near_v3(p1, p2)) {
                    idx_out.push_back(j);
                    find = true;
                    break;
                }
            }

            if (find)
                continue;

            // se diferente adiciona vertice e cria novo indice
            out.push_back(p1);
            idx_out.push_back(out.size() - 1);
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Simplify3 In: %04lu out: %04lu faces: %04lu ", in.size(), out.size(),
                     idx_out.size() / 3);
    }

    void mesh_serialize(Mesh& in_data, Mesh& out_data) {

        out_data.vertex.reserve(in_data.iFace.size() * 3); // Reserve Vertex(point, normal, tex)

        for (const glm::uvec3& face : in_data.iFace) {
            out_data.vertex.push_back(
                {in_data.vertex[face.x].point, in_data.vertex[face.x].normal, in_data.vertex[face.x].uv}); // Vertice A
            out_data.vertex.push_back(
                {in_data.vertex[face.y].point, in_data.vertex[face.y].normal, in_data.vertex[face.y].uv}); // Vertice B
            out_data.vertex.push_back(
                {in_data.vertex[face.z].point, in_data.vertex[face.z].normal, in_data.vertex[face.z].uv}); // Vertice C
        }

        for (uint32_t i = 0; i < out_data.vertex.size(); i += 3) // Serialize Vertex (0,1,2),(3,4,5),...
            out_data.iFace.push_back({i, i + 1, i + 2});

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Serialize Vertex: %04lu -> %04lu Faces: %04lu ", in_data.vertex.size(),
                     out_data.vertex.size(), out_data.iFace.size());
    }

    void mesh_reindex(Mesh& in_data, Mesh& out_data) {

        std::vector<uint32_t> idx_face;
        idx_face.reserve(in_data.iFace.size() * 3);
        for (const glm::uvec3& f : in_data.iFace) {
            idx_face.push_back(f.x);
            idx_face.push_back(f.y);
            idx_face.push_back(f.z);
        }

        std::vector<uint32_t> index;
        index.reserve(in_data.iFace.size() * 3);

        out_data.vertex.reserve(in_data.vertex.size());

        // percorrer todos os vertices
        bool find;
        for (uint32_t i = 0; i < idx_face.size(); i++) {
            // Procura por similar
            find = false;
            for (uint32_t j = 0; j < index.size(); j++) { // FIXME: trocar por vertexdata comp!!!!!
                if (is_near_v3(in_data.vertex[idx_face[i]].point, out_data.vertex[index[j]].point) && // compara pontos
                    is_near_v3(in_data.vertex[idx_face[i]].normal,
                               out_data.vertex[index[j]].normal) &&                             // compara normal
                    is_near_v2(in_data.vertex[idx_face[i]].uv, out_data.vertex[index[j]].uv)) { // compara uv

                    index.push_back(index[j]); // copia valor de index[j] que é repetido
                    find = true;
                    break;
                }
            }

            if (find)
                continue;

            // se diferente adiciona vertice e cria novo indice
            out_data.vertex.push_back({in_data.vertex[idx_face[i]].point,  //
                                       in_data.vertex[idx_face[i]].normal, //
                                       in_data.vertex[idx_face[i]].uv});   //

            index.push_back(out_data.vertex.size() - 1);
        }

        for (uint32_t i = 0; i < index.size(); i += 3)
            out_data.iFace.push_back({index[i], index[i + 1], index[i + 2]});

        out_data.iFace.assign(out_data.iFace.begin(), out_data.iFace.end());

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "ReIndex Vertex: %04lu -> %04lu Faces: %04lu ", in_data.vertex.size(),
                     out_data.vertex.size(), out_data.iFace.size());
    }

    void mesh_debug(const Mesh& m, bool show_all) {

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex : %03d, Faces  : %03d", (int)m.vertex.size(),
                     (int)m.iFace.size());
        if (show_all == true) {
            uint32_t i = 0;
            for (const VertexData& v : m.vertex)
                SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
                             "Vertex: %03d (%05.3f; %05.3f; %05.3f),(%02.3f; %02.3f; %02.3f), (%01.4f; %01.4f)", i++,
                             v.point.x, v.point.y, v.point.z, v.normal.x, v.normal.y, v.normal.z, v.uv.x, v.uv.y);

            i = 0;
            for (const glm::uvec3& face : m.iFace) {
                SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Index: %03d (%03d; %03d; %03d)", i, face.x, face.y, face.z);
                i += 3;
            }
        }
    }

    std::tuple<glm::vec3, glm::vec3, glm::vec3> vertex_boundaries(std::vector<VertexData>& v_array) {
        glm::vec3 min, max;
        if (v_array.size() > 0) {
            min = v_array[0].point;
            max = v_array[0].point;
        }

        for (const VertexData& v : v_array) {
            min = glm::min(min, v.point);
            max = glm::max(max, v.point);
        }

        return {min, max, get_size_min_max(min, max)};
    }

    std::tuple<glm::vec3, glm::vec3, glm::vec3> vertex_indexed_boundaries(std::vector<VertexData>& v_array,
                                                                          TrisIndex& tris) {
        glm::vec3 min, max;
        if (tris.size() > 0) {
            max = min = v_array[tris[0].x].point;
            max = min = v_array[tris[0].y].point;
            max = min = v_array[tris[0].z].point;
        }

        for (const glm::uvec3& i : tris) {
            min = glm::min(min, v_array[i.x].point);
            min = glm::min(min, v_array[i.y].point);
            min = glm::min(min, v_array[i.z].point);

            max = glm::max(max, v_array[i.x].point);
            max = glm::max(max, v_array[i.y].point);
            max = glm::max(max, v_array[i.z].point);
        }

        return {min, max, get_size_min_max(min, max)};
    }
} // namespace ce
