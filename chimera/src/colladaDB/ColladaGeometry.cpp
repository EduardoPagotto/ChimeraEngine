#include "chimera/colladaDB/ColladaGeometry.hpp"

namespace Chimera {
ColladaGeometry::~ColladaGeometry() {}

void ColladaGeometry::create(Entity& entity, pugi::xml_node nodeParent) { // FIXME: preciso mesmo da entidade ???

    // pugi::xml_node nodeGeo = n;
    //  TODO: continuar depois aqui!!
    //  InstanceCollada* novo = colladaURL(handle, "library_geometries", nodeGeo.attribute("url").value());
    //  if (novo != nullptr)
    //      handle = novo;

    // pugi::xml_node nodeLight = urlRoot(nodeParent, "library_lights", nodeParent.attribute("url").value());

    // ComponentLight& lc = entity.addComponent<ComponentLight>();
    // Light* light = new Light();

    // pugi::xml_node tec = nodeLight.child("technique_common");
    // for (pugi::xml_node lTec = tec.first_child(); lTec; lTec = lTec.next_sibling()) {

    //     std::string name = lTec.name();
    //     if (name == "point") {
    //         std::string color = lTec.child("color").text().as_string();
    //         light->setDiffuse(textToVec4(color));
    //         light->setType(LightType::POSITIONAL);

    //     } else if (name == "vector") {
    //         // TODO: implementar
    //     }
    // }

    // lc.light = light;
}

} // namespace Chimera