#include "chimera_core/gl/Wavefront.hpp"
#include "chimera_base/aux/utils.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include <fstream>

namespace ce {

    glm::vec4 tokens_to_vec4(std::vector<std::string>& text_data) {
        std::vector<float> array_float;
        for (int indice = 1; indice < text_data.size(); indice++) {
            if (text_data[indice].size() > 0)
                array_float.push_back(std::stod(text_data[indice]));
        }

        if (array_float.size() < 4)
            return glm::vec4(array_float[0], array_float[1], array_float[2], 1.0f);

        return glm::vec4(array_float[0], array_float[1], array_float[2], array_float[3]);
    }

    glm::vec3 tokens_to_vec3(unsigned start, unsigned total, std::vector<std::string>& text_data) {

        std::vector<float> array_float;
        for (int indice = start; array_float.size() < total; indice++) {
            if (text_data[indice].size() > 0)
                array_float.push_back(std::stod(text_data[indice]));
        }

        return glm::vec3(array_float[0], array_float[1], array_float[2]);
    }

    glm::vec2 tokens_to_vec2(unsigned start, unsigned total, std::vector<std::string>& text_data) {

        std::vector<float> array_float;
        for (int indice = start; array_float.size() < total; indice++) {
            if (text_data[indice].size() > 0)
                array_float.push_back(std::stod(text_data[indice]));
        }

        return glm::vec2(array_float[0], array_float[1]);
    }

    void WaveFront::wavefront_mtl_load(const std::string& path, std::shared_ptr<Material> material) {
        std::ifstream file(path);

        if (!file.is_open())
            throw std::string("ERROR: could not open file: " + path);

        std::string line_buffer;
        while (std::getline(file, line_buffer)) {
            std::string first = line_buffer.substr(0, 1);
            if (first == "#")
                continue;

            std::vector<std::string> text_data;
            text_to_string_array(line_buffer, text_data, ' ');

            if (text_data.size() == 0)
                continue;

            if (text_data[0] == "Ka") {
                material->set_ambient(tokens_to_vec4(text_data));
            } else if (text_data[0] == "Kd") {
                material->set_diffuse(tokens_to_vec4(text_data));
            } else if (text_data[0] == "Ks") {
                material->set_specular(tokens_to_vec4(text_data));
            } else if (text_data[0] == "map_Kd") {

                auto assets = this->registry_->ctx().get<std::shared_ptr<AssetManager>>();

                TexParam tp;
                material->add_texture(SHADE_TEXTURE_DIFFUSE,
                                      assets->load_texture(text_data[1], text_data[1], tp).handle());

            } else if (text_data[0] == "sharpness") {
                material->set_shine(std::stod(text_data[1]));
            }
        }
    }

    void WaveFront::wavefront_obj_load(const std::string& path, Mesh* mesh, std::string& file_math) {
        std::ifstream file(path);

        if (!file.is_open())
            throw std::string("ERROR: could not open file: " + path);

        std::vector<int> indices_comp;
        std::string line_buffer;
        std::vector<glm::vec3> point;
        std::vector<glm::vec3> normal;
        std::vector<glm::vec2> uv;

        while (std::getline(file, line_buffer)) {
            std::string first = line_buffer.substr(0, 1);
            if (first == "#")
                continue;

            std::vector<std::string> text_data;
            text_to_string_array(line_buffer, text_data, ' ');

            if (text_data.size() == 0)
                continue;

            if (text_data[0] == "mtllib")
                file_math = text_data[1];
            else if (text_data[0] == "v")
                point.push_back(tokens_to_vec3(1, 3, text_data));
            else if (text_data[0] == "vt")
                uv.push_back(tokens_to_vec2(1, 2, text_data));
            else if (text_data[0] == "vn")
                normal.push_back(tokens_to_vec3(1, 3, text_data));
            else if (text_data[0] == "f") {
                int face = 0;
                for (int indice = 1; indice < text_data.size(); indice++) {
                    if (text_data[indice].size() > 0) {
                        std::vector<std::string> ss;
                        if (face > 8)
                            break;

                        text_to_string_array(text_data[indice], ss, '/');
                        for (std::string cc : ss) {
                            if (cc.size() > 0) {
                                indices_comp.push_back(std::stod(cc) - 1);
                                face++;
                            }
                        }
                    }
                }
            }
        }

        std::vector<std::string> semantics;
        semantics.push_back("VERTEX"); // 0

        if (uv.size() > 0)
            semantics.push_back("TEXCOORD"); // 1

        if (normal.size() > 0)
            semantics.push_back("NORMAL"); // 2

        std::vector<uint32_t> i_point;
        std::vector<uint32_t> i_normal;
        std::vector<uint32_t> i_uv;

        for (uint32_t l_contador = 0; l_contador < indices_comp.size(); l_contador++) {

            uint32_t index = l_contador % semantics.size();
            const std::string& semantic = semantics[index];

            if (semantic == "VERTEX")
                i_point.push_back(indices_comp[l_contador]);
            else if (semantic == "NORMAL")
                i_normal.push_back(indices_comp[l_contador]);
            else if (semantic == "TEXCOORD")
                i_uv.push_back(indices_comp[l_contador]);
        }

        for (uint32_t face = 0; face < i_point.size(); face++) {
            mesh->vertex.push_back(
                {point[i_point[face]],                                     // point
                 normal[i_normal[face]],                                   // normal
                 (uv.size() > 0) ? uv[i_uv[face]] : glm::vec2(0.0, 0.0)}); // UV se nao existir zeros!!
        }

        for (uint32_t i = 0; i < i_point.size(); i += 3)
            mesh->iFace.push_back({i, i + 1, i + 2});

        file.close();
    }
} // namespace ce
