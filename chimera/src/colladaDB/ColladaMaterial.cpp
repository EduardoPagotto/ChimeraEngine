#include "chimera/colladaDB/ColladaMaterial.hpp"
#include "chimera/core/visible/TextureManager.hpp"

namespace Chimera {

ColladaMaterial::~ColladaMaterial() {}

Texture* ColladaMaterial::loadImage(pugi::xml_node nodeParent, const std::string& url) {

    pugi::xml_node node = urlRoot(nodeParent, "library_images", url);
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

    return nullptr;
}

void ColladaMaterial::create(Entity& entity, pugi::xml_node nodeParent) {

    std::unordered_map<std::string, Texture*> mapaTex;
    std::unordered_map<std::string, std::string> mapa2D;
    Material& eMaterial = entity.addComponent<Material>();

    pugi::xml_node instanceMaterial = nodeParent.child("bind_material").child("technique_common").child("instance_material");
    pugi::xml_node nodeMat = urlRoot(instanceMaterial, "library_materials", instanceMaterial.attribute("symbol").value());
    pugi::xml_node effect = nodeMat.child("instance_effect");
    pugi::xml_node novo2 = urlRoot(effect, "library_effects", effect.attribute("url").value());
    pugi::xml_node profile = novo2.first_child();

    for (pugi::xml_node param = profile.first_child(); param; param = param.next_sibling()) {

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
                        eMaterial.setEmission(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        // TODO: implementar
                    }

                } else if (p == "ambient") {
                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        eMaterial.setAmbient(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        // TODO: implementar
                    }
                } else if (p == "diffuse") {
                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        eMaterial.setDiffuse(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        std::string sAddr = first.attribute("texture").value();
                        std::string sAddr2 = mapa2D[sAddr];
                        Texture* tex = mapaTex[sAddr2];

                        eMaterial.addTexture(SHADE_TEXTURE_DIFFUSE, tex);
                    }
                } else if (p == "specular") {
                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "color")
                        eMaterial.setSpecular(textToVec4(first.text().as_string()));
                    else if (std::string(first.name()) == "texture") {
                        // TODO: implementar
                    }
                } else if (p == "shininess") {
                    // TODO: implementar

                    pugi::xml_node first = prop.first_child();
                    if (std::string(first.name()) == "float") {
                        float aa = first.text().as_float();
                        eMaterial.setShine(aa);
                    }

                } else if (p == "index_of_refraction") {
                    // TODO: implementar
                }
            }
        }
    }
}
} // namespace Chimera