#include "LibraryEffects.hpp"
#include "LibraryImages.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

LibraryEffects::LibraryEffects(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity)
    : Library(_root, _url), entity(entity) {}

LibraryEffects::~LibraryEffects() {}

void LibraryEffects::target() {

    for (tinyxml2::XMLElement* l_nEffect = root->FirstChildElement("library_effects")->FirstChildElement("effect"); l_nEffect != nullptr;
         l_nEffect = l_nEffect->NextSiblingElement()) {
        std::string l_id = l_nEffect->Attribute("id");
        if (url.compare(l_id) == 0) {

            tinyxml2::XMLElement* l_nProfile = l_nEffect->FirstChildElement("profile_COMMON");
            if (l_nProfile != nullptr) {

                loadNewParam(l_nProfile);

                MaterialComponent& eMaterial = entity.addComponent<MaterialComponent>();
                // eMaterial.material = new material;
                loadColors(l_nProfile, eMaterial.material);
                return;
            }
        }
    }

    throw std::string("Effect nao encontrado: " + url);
}

glm::vec4 LibraryEffects::getColor(tinyxml2::XMLElement* l_nColorVal) {

    std::vector<float> l_arrayF;
    const char* l_cor = l_nColorVal->GetText();
    loadArrayBtScalar(l_cor, l_arrayF);
    return glm::vec4(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0);
}

Texture* LibraryEffects::getTexture(tinyxml2::XMLElement* _nTex) {

    std::string texture1 = _nTex->Attribute("texture");

    auto sampler2D = mapaEfeito.find(texture1);
    if (sampler2D != mapaEfeito.end()) {

        auto sampler2DTxt = sampler2D->second;
        auto surface = mapaEfeito.find(sampler2DTxt);

        if (surface != mapaEfeito.end()) {

            auto textureName = surface->second;

            LibraryImages li(root, textureName);
            std::tuple<std::string, std::string> val = li.target();

            std::string valId = std::get<0>(val);
            std::string valPathFile = std::get<1>(val);

            TextureManager::loadFromFile(valId, valPathFile, TexParam());
            return TextureManager::getLast();
        }
    }

    throw std::string("Texture definido mas nao encontrado");
}

void LibraryEffects::loadColors(tinyxml2::XMLElement* _nProfile, Material* _pMat) {

    for (tinyxml2::XMLElement* l_nCor = _nProfile->FirstChildElement("technique")->FirstChildElement("phong")->FirstChildElement();
         l_nCor != nullptr; l_nCor = l_nCor->NextSiblingElement()) {

        glm::vec4 l_valCor;
        const char* l_cor = l_nCor->Value();

        tinyxml2::XMLElement* l_nColorVal = l_nCor->FirstChildElement("color");
        if (l_nColorVal != nullptr)
            l_valCor = getColor(l_nColorVal);

        tinyxml2::XMLElement* l_nTex = l_nCor->FirstChildElement("texture");
        if (l_nTex != nullptr) {

            Texture* pTex = getTexture(l_nTex);
            _pMat->addTexture(SHADE_TEXTURE_DIFFUSE, pTex); // FIXME: Textura tipo correta ????
        }

        if (strcmp(l_cor, (const char*)"emission") == 0) {

            if (l_nColorVal != nullptr)
                _pMat->setEmission(l_valCor);
            else if (l_nTex != nullptr)
                _pMat->setEmission(glm::vec4(1.0, 1.0, 1.0, 1.0));
            else
                throw std::string("Tipo de cor emission indefinida");

        } else if (strcmp(l_cor, (const char*)"ambient") == 0) {

            if (l_nColorVal != nullptr)
                _pMat->setAmbient(l_valCor);
            else if (l_nTex != nullptr)
                _pMat->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0));
            else
                throw std::string("Tipo de cor ambient indefinida");

        } else if (strcmp(l_cor, (const char*)"diffuse") == 0) {

            if (l_nColorVal != nullptr)
                _pMat->setDiffuse(l_valCor);
            else if (l_nTex != nullptr)
                _pMat->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0));
            else
                throw std::string("Tipo de cor diffuse indefinida");

        } else if (strcmp(l_cor, (const char*)"specular") == 0) {

            if (l_nColorVal != nullptr)
                _pMat->setSpecular(l_valCor);
            else if (l_nTex != nullptr)
                _pMat->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0));
            else
                throw std::string("Tipo de cor specular indefinida");

        } else if (strcmp(l_cor, (const char*)"shininess") == 0) {

            const char* l_val = l_nCor->FirstChildElement("float")->GetText();
            if (l_val != nullptr)
                _pMat->setShine(atof(l_val));
            else
                throw std::string("Tipo de cor shininess indefinida");
        }
    }
}

void LibraryEffects::loadNewParam(tinyxml2::XMLElement* _nProfile) {

    for (tinyxml2::XMLElement* l_nParam = _nProfile->FirstChildElement("newparam"); l_nParam; l_nParam = l_nParam->NextSiblingElement()) {
        std::string sid = l_nParam->Attribute("sid");

        tinyxml2::XMLElement* l_nSurface = l_nParam->FirstChildElement("surface");
        if (l_nSurface != nullptr) {

            std::string val = l_nSurface->FirstChildElement("init_from")->GetText();
            mapaEfeito.insert(std::pair<std::string, std::string>(sid, val));
        }

        tinyxml2::XMLElement* l_nSample2D = l_nParam->FirstChildElement("sampler2D");
        if (l_nSample2D != nullptr) {

            std::string val = l_nSample2D->FirstChildElement("source")->GetText();
            mapaEfeito.insert(std::pair<std::string, std::string>(sid, val));
        }
    }
}
} // namespace Chimera