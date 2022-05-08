#include "chimera/core/collada/ColladaLight.hpp"
#include "chimera/core/visible/Light.hpp"

namespace Chimera {
ColladaLight::~ColladaLight() {}

void ColladaLight::create(Entity& entity, pugi::xml_node nodeLight) { // FIXME: preciso mesmo da entidade ???
    LightComponent& lc = entity.addComponent<LightComponent>();
    lc.tag.id = nodeLight.attribute("id").value();
    lc.tag.tag = nodeLight.attribute("name").value();
    lc.tag.serial = Collada::getNewSerial();

    pugi::xml_node tec = nodeLight.child("technique_common");
    for (pugi::xml_node lTec = tec.first_child(); lTec; lTec = lTec.next_sibling()) {

        std::string name = lTec.name();
        if (name == "point") {
            std::string color = lTec.child("color").text().as_string();
            lc.light->setDiffuse(textToVec4(color));
            lc.light->setType(LightType::POSITIONAL);

        } else if (name == "directional") {
            // TODO: implementar
            std::string color = lTec.child("color").text().as_string();
            lc.light->setDiffuse(textToVec4(color));
            lc.light->setType(LightType::DIRECTIONAL);
        }
    }
}

} // namespace Chimera