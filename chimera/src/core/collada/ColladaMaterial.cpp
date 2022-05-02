#include "chimera/core/collada/ColladaMaterial.hpp"
#include "chimera/core/collada/ColladaImage.hpp"
#include "chimera/core/collada/ColladaShader.hpp"
#include "chimera/core/visible/Components.hpp"

namespace Chimera {

ColladaMaterial::~ColladaMaterial() {}

void ColladaMaterial::createEffect(Material* pMat, pugi::xml_node nodeParent) {

    ColladaImage ci(colladaDom, "#local");
    ci.create(nodeParent);

    for (pugi::xml_node param = nodeParent.first_child(); param; param = param.next_sibling()) {

        std::string sProf = param.name();
        std::string sid = param.attribute("sid").value();
        if (sProf == "technique") {

            pugi::xml_node phong = param.child("phong");
            for (pugi::xml_node prop = phong.first_child(); prop; prop = prop.next_sibling()) {

                std::string p = prop.name();
                if (p == "emission") {

                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        pMat->setEmission(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        // TODO: implementar
                    }

                } else if (p == "ambient") {
                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        pMat->setAmbient(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        // TODO: implementar
                    }
                } else if (p == "diffuse") {
                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        pMat->setDiffuse(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        pMat->addTexture(SHADE_TEXTURE_DIFFUSE, ci.getTexureByName(first.attribute("texture").value()));
                        pMat->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // FIXME: Arquivo do blender nao tem!!
                    }
                } else if (p == "specular") {
                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        pMat->setSpecular(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        // TODO: implementar
                    }
                } else if (p == "shininess") {
                    // TODO: implementar

                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "float") {
                        float aa = first.text().as_float();
                        pMat->setShine(aa);
                    }

                } else if (p == "index_of_refraction") {
                    // TODO: implementar
                }
            }
        }
    }
}

void ColladaMaterial::create(Entity& entity, pugi::xml_node nodeParent) {

    std::string refName = "";

    pugi::xml_node material = getLibrary("library_materials", nodeParent.attribute("symbol").value());
    pugi::xml_node instanceEffect = material.child("instance_effect");
    pugi::xml_node technique_hint = instanceEffect.child("technique_hint");

    if (technique_hint != nullptr) {
        if (std::string(technique_hint.attribute("profile").value()) == "GLSL")
            refName = technique_hint.attribute("ref").value();
    }

    pugi::xml_node effect = getLibrary("library_effects", instanceEffect.attribute("url").value());

    ComponentMaterial& eMaterial = entity.addComponent<ComponentMaterial>();
    eMaterial.tag.id = material.attribute("id").value();
    eMaterial.tag.tag = material.attribute("name").value();
    eMaterial.tag.serial = Collada::getNewSerial();

    pugi::xml_node profile = effect.child("profile_COMMON");
    if (profile != nullptr)
        createEffect(eMaterial.material, profile);

    pugi::xml_node extra = effect.child("extra");
    if (extra != nullptr) {

        pugi::xml_node instanceEffectShade = extra.child("instance_effect");
        std::string url = instanceEffectShade.attribute("url").value();

        ColladaShader cs(colladaDom, url);
        cs.create(refName, entity, cs.getLibrary("library_effects", url));

    } else {

        pugi::xml_node profileGLSL = effect.child("profile_GLSL");
        if (profileGLSL != nullptr) {

            ColladaShader cs(colladaDom, refName);
            cs.create(refName, entity, effect); // get profileGLSL inside here
        }
    }
}
} // namespace Chimera