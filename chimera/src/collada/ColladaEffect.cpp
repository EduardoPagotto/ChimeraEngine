#include "chimera/core/collada/ColladaEffect.hpp"
#include "chimera/core/collada/ColladaImage.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Shader.hpp"
#include "chimera/core/visible/ShaderMng.hpp"

namespace ce {

static TexFilter setFilter(const std::string& sParamVal) {
    if (sParamVal == "NEAREST")
        return TexFilter::NEAREST;
    else if (sParamVal == "LINEAR")
        return TexFilter::LINEAR;

    return TexFilter::NONE;
}

static TexWrap setWrap(const std::string& sParamVal) {
    if (sParamVal == "WRAP")
        return TexWrap::REPEAT;
    else if (sParamVal == "MIRROR")
        return TexWrap::MIRRORED;
    else if (sParamVal == "CLAMP")
        return TexWrap::CLAMP_TO_EDGE;
    else if (sParamVal == "CLAMP2")
        return TexWrap::CLAMP;
    else if (sParamVal == "BORDER")
        return TexWrap::CLAMP_TO_BORDER;

    return TexWrap::NONE;
}

void ColladaEffect::setShader(const std::string& refName, const pugi::xml_node& node) {
    pugi::xml_node tech = node.child("technique");

    if ((refName.size() > 0) && (std::string(tech.attribute("sid").value()) != refName))
        return;

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
        auto mng = g_service_locator.getService<ShaderMng>();
        entity.addComponent<ShaderComponent>(refName, mng->load(refName, shadeData));
    }
}

bool ColladaEffect::setTextureParam(const pugi::xml_node& n, TexParam& tp) {
    for (pugi::xml_node ntPara = n.first_child(); ntPara; ntPara = ntPara.next_sibling()) {
        std::string sParam = ntPara.name();
        std::string sParamVal = ntPara.text().as_string();
        if (sParam == "minfilter")
            tp.minFilter = setFilter(sParamVal);
        else if (sParam == "magfilter")
            tp.magFilter = setFilter(sParamVal);
        else if (sParam == "wrap_s")
            tp.wrap_s = setWrap(sParamVal);
        else if (sParam == "wrap_t")
            tp.wrap_t = setWrap(sParamVal);
        else if (sParam == "wrap_r")
            tp.wrap_r = setWrap(sParamVal);
        else if (sParam == "instance_image") {

            std::string url = ntPara.attribute("url").value();
            ColladaImage ci(colladaDom, url);
            ci.create(entity, tp, ci.getLibrary("library_images"));
            return true;
        }
    }
    return false;
}

void ColladaEffect::setMaterial(const pugi::xml_node& node, TexParam& tp) {

    Material* pMat = nullptr;
    if (entity.hasComponent<MaterialComponent>()) {
        MaterialComponent& mc = entity.getComponent<MaterialComponent>();
        pMat = mc.material;
    } else
        return;

    pugi::xml_node phong = node.child("phong");
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

                std::string texId = first.attribute("texture").value();
                std::string idTex = mapaTex[mapa2D[texId]];

                ColladaImage ci(colladaDom, idTex);
                ci.create(entity, tp, ci.getLibrary("library_images"));

                auto texMng = g_service_locator.getService<TextureMng>();
                pMat->addTexture(SHADE_TEXTURE_DIFFUSE, texMng->get(idTex));
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

void ColladaEffect::setImageParms(const pugi::xml_node& node) {

    for (pugi::xml_node param = node.first_child(); param; param = param.next_sibling()) {

        TexParam tp;
        std::string sProf = param.name();
        std::string sid = param.attribute("sid").value();
        if (sProf == "newparam") {

            pugi::xml_node val1 = param.first_child();
            if (std::string sVal1 = val1.name(); sVal1 == "surface") {
                std::string keyImage = val1.child("init_from").text().as_string();
                // loadImage(keyImage, tp);
                mapaTex[sid] = keyImage;

            } else if (sVal1 == "sampler2D") {
                if (setTextureParam(val1, tp) == false) {
                    std::string keyMap = val1.child("source").text().as_string();
                    mapa2D[sid] = keyMap;
                }
            } else if (sVal1 == "samplerDEPTH") {
                // nao e textura e FR
                tp.format = TexFormat::DEPTH_COMPONENT;
                tp.internalFormat = TexFormat::DEPTH_COMPONENT;
                setTextureParam(val1, tp);
            }
        } else if (sProf == "technique") {
            setMaterial(param, tp);
        }
    }
}

void ColladaEffect::create(const std::string& refName, Entity& entity, pugi::xml_node node) {
    this->entity = entity;
    for (pugi::xml_node nProf = node.first_child(); nProf; nProf = nProf.next_sibling()) {
        if (std::string nameProf = nProf.name(); nameProf == "profile_GLSL") {
            setShader(refName, nProf);
        } else if (nameProf == "profile_COMMON") {
            setImageParms(nProf);
        } else if (nameProf == "extra") {
            if (const pugi::xml_node nFX = getExtra(nProf, "instance_effect"); nFX != nullptr) {
                std::string url = nFX.attribute("url").value();
                ColladaEffect cf(colladaDom, url);
                cf.create("", entity, cf.getLibrary("library_effects"));
            }
        }
    }
}
} // namespace ce