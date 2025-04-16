#include "chimera/collada/ColladaMaterial.hpp"
#include "chimera/collada/ColladaEffect.hpp"
#include "chimera/core/gl/Material.hpp"

namespace ce {
void ColladaMaterial::create(Entity& entity, const pugi::xml_node& node) {

    MaterialComponent& eMaterial = entity.addComponent<MaterialComponent>();
    eMaterial.tag.id = node.attribute("id").value();
    eMaterial.tag.tag = node.attribute("name").value();
    eMaterial.tag.serial = Collada::getNewSerial();

    pugi::xml_node nEffect = node.child("instance_effect");
    std::string url = nEffect.attribute("url").value();
    std::string refName = nEffect.child("technique_hint").attribute("ref").value();

    ColladaEffect cf(colladaDom, url);
    cf.create(refName, entity, cf.getLibrary("library_effects"));
}
} // namespace ce