#include "chimera/core/collada/ColladaWaveFront.hpp"
#include "chimera/core/collada/ColladaEffect.hpp"
#include "chimera/core/partition/Wavefront.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace ce {
void ColladaWaveFront::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo) {

    MeshComponent& eMesh = entity.addComponent<MeshComponent>();
    eMesh.mesh = new Mesh();
    eMesh.tag.id = id;
    eMesh.tag.tag = name;
    eMesh.tag.serial = Collada::getNewSerial();
    eMesh.type = getMeshTypeFromString(geo.attribute("partition").value());
    std::string target = geo.attribute("target").value();

    MaterialComponent& eMaterial = entity.addComponent<MaterialComponent>();

    eMaterial.tag.id = eMesh.tag.id + "_mat";
    eMaterial.tag.tag = eMesh.tag.tag + "_mat";
    eMaterial.tag.serial = Collada::getNewSerial();

    std::string matFile;
    wavefrontObjLoad(target, eMesh.mesh, matFile);
    if (matFile.size() > 0) {
        wavefrontMtlLoad(matFile, eMaterial.material);
    }

    if (pugi::xml_node nShade = geo.next_sibling(); nShade) {
        if (pugi::xml_node technique_hint = nShade.child("technique_hint"); technique_hint != nullptr) {
            if (std::string(technique_hint.attribute("profile").value()) == "GLSL") {
                std::string refName = technique_hint.attribute("ref").value();
                std::string url = nShade.attribute("url").value();
                ColladaEffect cf(colladaDom, url);

                cf.create(refName, entity, cf.getLibrary("library_effects"));
            }
        }
    }
}
} // namespace ce