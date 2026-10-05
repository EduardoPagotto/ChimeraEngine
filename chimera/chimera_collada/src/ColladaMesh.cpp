#include "chimera_collada/ColladaMesh.hpp"
#include "chimera_base/aux/utils.hpp"
#include "chimera_ecs/MeshComponent.hpp"

namespace ce {
    void ColladaMesh::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node n_mesh) {

        MeshComponent& e_mesh = entity.add_component<MeshComponent>(registry.get());
        e_mesh.mesh = new Mesh();
        e_mesh.tag.id = id;
        e_mesh.tag.name = name;
        // eMesh.tag.serial = Collada::getNewSerial();

        std::vector<glm::vec3> point;
        std::vector<glm::vec3> normal;
        std::vector<glm::vec2> uv;

        for (pugi::xml_node source = n_mesh.first_child(); source; source = source.next_sibling()) {

            std::string name = source.name();
            std::string id = source.attribute("id").value();

            if (name == "source") {

                std::vector<float> v;
                pugi::xml_node n_list = source.child("float_array");
                text_to_float_array(n_list.text().as_string(), v);

                if (id.find("-positions") != std::string::npos) {

                    for (size_t indice = 0; indice < v.size(); indice += 3)
                        point.push_back(glm::vec3(v[indice], v[indice + 1], v[indice + 2]));

                } else if (id.find("-normals") != std::string::npos) {

                    for (size_t indice = 0; indice < v.size(); indice += 3)
                        normal.push_back(glm::vec3(v[indice], v[indice + 1], v[indice + 2]));

                } else if (id.find("-map-0") != std::string::npos) {

                    for (size_t indice = 0; indice < v.size(); indice += 2)
                        uv.push_back(glm::vec2(v[indice], v[indice + 1]));
                }

            } else if (name == "vertices") {
            } else if (name == "polylist") {

                std::vector<std::string> semantics;

                for (pugi::xml_node n_input = source.first_child(); n_input; n_input = n_input.next_sibling()) {
                    std::string input_name = n_input.name();
                    if (input_name == "input") {

                        semantics.push_back(n_input.attribute("semantic").value());

                    } else if (input_name == "vcount") {
                    } else if (input_name == "p") {

                        std::vector<uint32_t> array_index;
                        text_to_u_int_array(n_input.text().as_string(), array_index);

                        std::vector<uint32_t> i_point;
                        std::vector<uint32_t> i_normal;
                        std::vector<uint32_t> i_uv;

                        for (uint32_t l_contador = 0; l_contador < array_index.size(); l_contador++) {

                            uint32_t index = l_contador % semantics.size();
                            const std::string& semantic = semantics[index];

                            if (semantic == "VERTEX") {
                                i_point.push_back(array_index[l_contador]);
                            } else if (semantic == "NORMAL") {
                                i_normal.push_back(array_index[l_contador]);
                            } else if (semantic == "TEXCOORD") {
                                i_uv.push_back(array_index[l_contador]);
                            }
                        }

                        for (uint32_t face = 0; face < i_point.size(); face++) {
                            e_mesh.mesh->vertex.push_back(
                                {point[i_point[face]],                                     // point
                                 normal[i_normal[face]],                                   // normal
                                 (uv.size() > 0) ? uv[i_uv[face]] : glm::vec2(0.0, 0.0)}); // UV se nao existir zeros!!
                        }

                        for (uint32_t i = 0; i < i_point.size(); i += 3)
                            e_mesh.mesh->iFace.push_back({i, i + 1, i + 2});

                        array_index.clear();
                    }
                }
                semantics.clear();
            }
        }
    }
} // namespace ce
