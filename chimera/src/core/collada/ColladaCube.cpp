#include "chimera/core/collada/ColladaCube.hpp"
#include "chimera/core/partition/Cube.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace Chimera {

ColladaCube::~ColladaCube() {}

void ColladaCube::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo) {

    MeshComponent& mc = entity.addComponent<MeshComponent>();
    mc.tag.id = id;
    mc.tag.tag = name;
    mc.tag.serial = Collada::getNewSerial();
    mc.type = MeshType::BSTREE; // FIXME: Aqui mesmo ?????

    uint32_t width = static_cast<uint32_t>(std::stoul(geo.attribute("width").value()));
    uint32_t height = static_cast<uint32_t>(std::stoul(geo.attribute("height").value()));
    uint32_t floor = static_cast<uint32_t>(std::stoul(geo.attribute("floor").value()));
    float sizeBlock = std::stod(geo.attribute("size").value());

    // carregando campos do mapa
    pugi::xml_node nl = geo.first_child();
    std::vector<Cube*> vpCube;
    glm::ivec3 pos(0);
    glm::ivec3 size(width, floor, height);
    glm::vec3 halfBlock((size.x * sizeBlock) / 2.0f,  //(w/2)
                        (size.y * sizeBlock) / 2.0f,  //(d/2)
                        (size.z * sizeBlock) / 2.0f); //(h/2)

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

            std::string sBuffer = nl.text().as_string();
            const char* buffer = sBuffer.c_str();

            for (pos.x = 0; pos.x < size.x; pos.x++) {

                Cube* pCube = nullptr;
                glm::vec3 min = minimal(sizeBlock, halfBlock, pos);
                glm::vec3 max = min + sizeBlock;

                if (sBuffer.size() > pos.x)
                    pCube = new Cube(buffer[pos.x], min, max);
                else
                    pCube = new Cube(' ', min, max); // Erro campo faltando

                vpCube.push_back(pCube);
            }

            nl = nl.next_sibling();
        }
    }

    linkCubes(size, vpCube);

    // carrega posicoes, texturas, e seq textura defaults do cubo base
    initCubeBase();
    for (auto pCube : vpCube)
        pCube->create(mc.mesh); // cria mesh com dados dos cubos

    cleanupCubeBase();          // limpa dados de criacao do cubo base
    for (auto pCube : vpCube) { // limpas cubos de contrucao e vetor de cubos
        delete pCube;
        pCube = nullptr;
    }
    vpCube.clear();
}
} // namespace Chimera