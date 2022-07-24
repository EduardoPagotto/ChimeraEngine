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
    uint32_t deep = static_cast<uint32_t>(std::stoul(geo.attribute("deep").value()));
    float sizeBlock = std::stod(geo.attribute("size").value());
    glm::ivec3 size(width, deep, height);

    std::string target = geo.attribute("target").value();
    // sizeBlock = geo.text().as_float();

    // processa o Maze
    std::vector<Cube*> vpCube;
    size = createMazeCube(target.c_str(), sizeBlock, vpCube);
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
    // createMap(mc.mesh, vpCube);
}
} // namespace Chimera