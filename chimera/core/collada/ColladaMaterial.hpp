#pragma once
#include "../Registry.hpp"
#include "Collada.hpp"
#include "chimera/core/Material.hpp"
#include "chimera/core/collada/ColladaEffect.hpp"

namespace Chimera {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial(ColladaDom& dom, const std::string& url) : Collada(dom, url){};

    virtual ~ColladaMaterial() {}

    void create(Entity& entity, const pugi::xml_node& node) {

        MaterialComponent& eMaterial = entity.addComponent<MaterialComponent>();
        eMaterial.tag.id = node.attribute("id").value();
        eMaterial.tag.tag = node.attribute("name").value();
        eMaterial.tag.serial = Collada::getNewSerial();

        pugi::xml_node nEffect = node.child("instance_effect");
        std::string url = nEffect.attribute("url").value();
        std::string refName = nEffect.child("technique_hint").attribute("ref").value();

        ColladaEffect cf(m_colladaDom, url);
        cf.create(refName, entity, cf.getLibrary("library_effects"));
    }
};
} // namespace Chimera
