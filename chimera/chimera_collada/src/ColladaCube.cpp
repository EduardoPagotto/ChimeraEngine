#include "chimera_collada/ColladaCube.hpp"
#include "chimera_core/partition/Cube.hpp"
#include "chimera_ecs/MeshComponent.hpp"

namespace ce {
    void ColladaCube::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo) {

        MeshComponent& mc = entity.add_component<MeshComponent>(registry.get());
        mc.tag.id = id;
        mc.tag.name = name;
        // mc.tag.serial = Collada::getNewSerial();
        mc.type = get_mesh_type_from_string(geo.attribute("partition").value());

        uint32_t width = static_cast<uint32_t>(std::stoul(geo.attribute("width").value()));
        uint32_t height = static_cast<uint32_t>(std::stoul(geo.attribute("height").value()));
        uint32_t floor = static_cast<uint32_t>(std::stoul(geo.attribute("floor").value()));
        float size_block = std::stod(geo.attribute("size").value());

        // carregando campos do mapa
        pugi::xml_node nl = geo.first_child();
        std::vector<Cube*> vp_cube;
        glm::ivec3 pos(0);
        glm::ivec3 size(width, floor, height);
        glm::vec3 half_block((size.x * size_block) / 2.0f,  //(w/2)
                             (size.y * size_block) / 2.0f,  //(d/2)
                             (size.z * size_block) / 2.0f); //(h/2)

        // processa o Maze
        for (pos.y = 0; pos.y < size.y; pos.y++) {
            // valida se floor esta correto
            uint32_t f = static_cast<uint32_t>(std::stoul(nl.attribute("f").value()));
            if (f != pos.y)
                throw std::string("floor do maze incorreto");

            for (pos.z = 0; pos.z < size.z; pos.z++) {
                // se numero de height for menor que o correto pular para p proximo
                if (nl == nullptr)
                    continue;
                // Valida de height esta correto
                uint32_t h = static_cast<uint32_t>(std::stoul(nl.attribute("h").value()));
                if (h != pos.z)
                    throw std::string("height do maze incorreto");

                std::string s_buffer = nl.text().as_string();
                const char* buffer = s_buffer.c_str();

                for (pos.x = 0; pos.x < size.x; pos.x++) {

                    Cube* p_cube = nullptr;
                    glm::vec3 min = minimal(size_block, half_block, pos);
                    glm::vec3 max = min + size_block;

                    if (s_buffer.size() > pos.x)
                        p_cube = new Cube(buffer[pos.x], min, max);
                    else
                        p_cube = new Cube(' ', min, max); // Erro campo faltando

                    vp_cube.push_back(p_cube);
                }

                nl = nl.next_sibling();
            }
        }

        linkCubes(size, vp_cube);

        // carrega posicoes, texturas, e seq textura defaults do cubo base
        initCubeBase();
        Mesh temp_mesh;
        for (auto p_cube : vp_cube)
            p_cube->create(&temp_mesh); // cria mesh com dados dos cubos

        // aqui
        mesh_serialize(temp_mesh, *mc.mesh);

        cleanupCubeBase();            // limpa dados de criacao do cubo base
        for (auto p_cube : vp_cube) { // limpas cubos de contrucao e vetor de cubos
            delete p_cube;
            p_cube = nullptr;
        }
        vp_cube.clear();
    }
} // namespace ce
