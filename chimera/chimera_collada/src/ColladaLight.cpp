#include "chimera_collada/ColladaLight.hpp"
#include "chimera_ecs/LightComponent.hpp"

namespace ce {
    void ColladaLight::create(Entity& entity, pugi::xml_node node_light) { // FIXME: preciso mesmo da entidade ???
        LightComponent& lc = entity.add_component<LightComponent>(registry.get());
        lc.light = std::make_shared<Light>();
        lc.tag.id = node_light.attribute("id").value();
        lc.tag.name = node_light.attribute("name").value();
        // lc.tag.serial = Collada::getNewSerial();

        pugi::xml_node tec = node_light.child("technique_common");
        for (pugi::xml_node l_tec = tec.first_child(); l_tec; l_tec = l_tec.next_sibling()) {

            if (std::string name = l_tec.name(); name == "point") {
                std::string color = l_tec.child("color").text().as_string();
                lc.light->set_diffuse(textToVec4(color));
                lc.light->set_ambient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)); // FIXME: remover depois
                lc.light->set_type(LightType::POSITIONAL);

            } else if (name == "directional") {
                // TODO: implementar
                std::string color = l_tec.child("color").text().as_string();
                lc.light->set_diffuse(textToVec4(color));
                lc.light->set_ambient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)); // FIXME: remover depois
                lc.light->set_type(LightType::DIRECTIONAL);
            }
        }
    }

} // namespace ce
