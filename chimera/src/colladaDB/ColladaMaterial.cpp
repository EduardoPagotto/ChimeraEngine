#include "chimera/colladaDB/ColladaMaterial.hpp"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/Shader.hpp"
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

    throw std::string("Textura nao encontrada: " + url);
}

void ColladaMaterial::createShader(const std::string& refName, Entity& entity, pugi::xml_node nodeParent) {

    pugi::xml_node tech = nodeParent.child("technique");

    if (std::string(tech.attribute("sid").value()) == refName) {

        std::unordered_map<std::string, std::string> mapaShaderFiles;
        std::unordered_map<GLenum, std::string> shadeData;

        for (pugi::xml_node item = tech.first_child(); item; item = item.next_sibling()) {

            std::string name = item.name();
            if (name == "include") {

                std::string url = std::string(item.attribute("url").value());
                const char* urlFile = "file://";
                size_t urlFileLen = strlen(urlFile);
                std::size_t found = url.find(urlFile, 0, urlFileLen);
                if (found != std::string::npos) {
                    std::string file = url.substr(urlFileLen, std::string::npos);
                    mapaShaderFiles[std::string(item.attribute("sid").value())] = file;
                } else {
                    throw std::string("Url de arquivo Shader invalido: %s", url.c_str());
                }

            } else if (name == "pass") {
                std::string sid = item.attribute("sid").value();

                for (pugi::xml_node pass = item.first_child(); pass; pass = pass.next_sibling()) {
                    if (std::string(pass.name()) == "shader") {

                        std::string stage = pass.attribute("stage").value();
                        std::string source = pass.child("name").attribute("source").value();

                        if ((stage.size() > 0) && (source.size() > 0)) {
                            if (stage == "VERTEXPROGRAM") {
                                shadeData[GL_VERTEX_SHADER] = mapaShaderFiles[source];
                            } else if (stage == "FRAGMENTPROGRAM") {
                                shadeData[GL_FRAGMENT_SHADER] = mapaShaderFiles[source];
                            }
                        }
                    }
                }
            }
        }

        if (shadeData.size() > 1) {
            Shader& shader = entity.addComponent<Shader>();
            ShaderManager::load(refName, shadeData, shader);
        }
    }
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

    pugi::xml_node material = urlRoot(nodeParent, "library_materials", nodeParent.attribute("symbol").value());
    pugi::xml_node instanceEffect = material.child("instance_effect");
    pugi::xml_node technique_hint = instanceEffect.child("technique_hint");

    if (technique_hint != nullptr) {
        if (std::string(technique_hint.attribute("profile").value()) == "GLSL")
            refName = technique_hint.attribute("ref").value();
    }

    pugi::xml_node effect = urlRoot(instanceEffect, "library_effects", instanceEffect.attribute("url").value());

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
        pugi::xml_node effectShade = urlRoot(instanceEffectShade, "library_effects", instanceEffectShade.attribute("url").value());

        pugi::xml_node profileGLSL = effectShade.child("profile_GLSL");
        if (profileGLSL != nullptr)
            createShader(refName, entity, profileGLSL);
    }
}
} // namespace Chimera