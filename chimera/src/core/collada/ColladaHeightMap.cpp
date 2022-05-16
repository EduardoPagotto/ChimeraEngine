#include "chimera/core/collada/ColladaHeightMap.hpp"
#include "chimera/core/partition/LoadHeightMap.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace Chimera {

ColladaHeightMap::~ColladaHeightMap() {}

void ColladaHeightMap::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo) {

    MeshComponent& mc = entity.addComponent<MeshComponent>();
    mc.tag.id = id;
    mc.tag.tag = name;
    mc.tag.serial = Collada::getNewSerial();

    std::string target = geo.attribute("target").value();
    uint32_t square_x = static_cast<uint32_t>(std::stoul(geo.attribute("square_x").value()));
    uint32_t square_y = static_cast<uint32_t>(std::stoul(geo.attribute("square_y").value()));
    glm::vec3 size = textToVec3(geo.next_sibling("size").text().as_string());

    LoadHeightMap loader(32, 32);
    loader.getMesh(target, *mc.mesh, size);
    loader.split(mc.mesh->iPoint);
    mc.vTrisIndex = loader.vNodes;
    mc.type = MeshType::ARRAY;
}
} // namespace Chimera