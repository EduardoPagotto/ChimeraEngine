#include "chimera/colladaDB/ColladaMaterial.hpp"
#include "chimera/colladaDB/ColladaShader.hpp"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/TextureManager.hpp"

namespace Chimera {

ColladaMaterial::~ColladaMaterial() {}

Texture* ColladaMaterial::loadImage(pugi::xml_node nodeParent, const std::string& url) {

    pugi::xml_node node = getLibrary("library_images", url); // FIXME!!!
    pugi::xml_node init = node.child("init_from");
    if (init != nullptr) {
        pugi::xml_text pathFile = init.text();
        if (pathFile != nullptr) {
            std::string f = pathFile.as_string();
            SDL_Log("Nova textura %s, Key: %s", f.c_str(), url.c_str());

            TextureManager::loadFromFile(url, f, TexParam());
            return TextureManager::getLast();
        }
    }

    throw std::string("Textura nao encontrada: " + url);
}

void ColladaMaterial::createEffect(Material* pMat, pugi::xml_node nodeParent) {

    std::unordered_map<std::string, Texture*> mapaTex;
    std::unordered_map<std::string, std::string> mapa2D;

    for (pugi::xml_node param = nodeParent.first_child(); param; param = param.next_sibling()) {

        std::string sProf = param.name();
        std::string sid = param.attribute("sid").value();
        if (sProf == "newparam") {

            pugi::xml_node val1 = param.first_child();
            std::string sVal1 = val1.name();

            if (sVal1 == "surface") {

                std::string keyImage = val1.child("init_from").text().as_string();
                Texture* tex = loadImage(val1, keyImage);
                mapaTex[sid] = tex;

            } else if (sVal1 == "sampler2D") {

                std::string keyMap = val1.child("source").text().as_string();
                mapa2D[sid] = keyMap;
            }

        } else if (sProf == "technique") {

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
                        std::string sAddr = first.attribute("texture").value();
                        std::string sAddr2 = mapa2D[sAddr];
                        Texture* tex = mapaTex[sAddr2];

                        pMat->addTexture(SHADE_TEXTURE_DIFFUSE, tex);
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
            cs.create(refName, entity, profileGLSL);
        }
    }
}
} // namespace Chimera