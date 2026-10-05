#include "chimera_collada/ColladaMaterial.hpp"
#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_ecs/MaterialComponent.hpp"

namespace ce {
    void ColladaMaterial::create(Entity& entity, const pugi::xml_node& node) {

        MaterialComponent& e_material = entity.add_component<MaterialComponent>(registry.get());
        e_material.tag.id = node.attribute("id").value();
        e_material.tag.name = node.attribute("name").value();
        e_material.material = std::make_shared<Material>();

        pugi::xml_node n_effect = node.child("instance_effect");
        std::string url = n_effect.attribute("url").value();
        std::string ref_name = n_effect.child("technique_hint").attribute("ref").value();

        ColladaEffect cf(registry, colladaDom, url);
        cf.create(ref_name, entity, cf.get_library("library_effects"));
    }
} // namespace ce
