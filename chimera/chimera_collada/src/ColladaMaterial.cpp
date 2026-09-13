#include "chimera_collada/ColladaMaterial.hpp"
#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_ecs/MaterialComponent.hpp"

namespace ce {
    void ColladaMaterial::create(Entity& entity, const pugi::xml_node& node) {

        MaterialComponent& eMaterial = entity.addComponent<MaterialComponent>();
        eMaterial.tag.id = node.attribute("id").value();
        eMaterial.tag.name = node.attribute("name").value();
        eMaterial.material = std::make_shared<Material>();

        pugi::xml_node nEffect = node.child("instance_effect");
        std::string url = nEffect.attribute("url").value();
        std::string refName = nEffect.child("technique_hint").attribute("ref").value();

        ColladaEffect cf(colladaDom, url);
        cf.create(refName, entity, cf.getLibrary("library_effects"));
    }
} // namespace ce
