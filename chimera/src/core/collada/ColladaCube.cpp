#include "chimera/core/collada/ColladaCube.hpp"
#include "chimera/core/partition/Maze.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace Chimera {

ColladaCube::~ColladaCube() {}

void ColladaCube::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo) {

    MeshComponent& mc = entity.addComponent<MeshComponent>();
    mc.tag.id = id;
    mc.tag.tag = name;
    mc.tag.serial = Collada::getNewSerial();
    mc.type = MeshType::BSTREE; // FIXME: Aqui mesmo ?????

    std::string target = geo.attribute("target").value();
    float size = geo.text().as_float();

    // processa o Maze
    Maze maze = Maze(target.c_str(), mc.mesh, size);
    maze.createMap();
}
} // namespace Chimera