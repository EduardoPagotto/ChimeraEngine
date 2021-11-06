#include "LibraryLights.hpp"
#include "chimera/core/Exception.hpp"
#include <tuple>

namespace Chimera {

LibraryLights::LibraryLights(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryLights::~LibraryLights() {}

Chimera::NodeLight* LibraryLights::target() {

    tinyxml2::XMLElement* l_nLight = root->FirstChildElement("library_lights")->FirstChildElement("light");
    for (l_nLight; l_nLight; l_nLight = l_nLight->NextSiblingElement()) {

        std::string l_id = l_nLight->Attribute("id");
        if (url.compare(l_id) == 0) {

            Chimera::NodeLight* pLight = new Chimera::NodeLight(nullptr, l_id);

            auto ret_data = loadDiffuseLightColor(l_nLight);
            pLight->data.setDiffuse(std::get<0>(ret_data));
            pLight->data.setType(std::get<1>(ret_data));

            return pLight;
        }
    }

    throw Chimera::Exception("NodeLight nao encontrada: " + url);
}

glm::vec4 LibraryLights::getColor(tinyxml2::XMLElement* l_nColorVal) {

    std::vector<float> l_arrayF;
    const char* l_val = l_nColorVal->FirstChildElement("color")->GetText();
    loadArrayBtScalar(l_val, l_arrayF);
    return glm::vec4(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);
}

std::tuple<glm::vec4, Chimera::LightType> LibraryLights::loadDiffuseLightColor(tinyxml2::XMLElement* _nNode) {
    tinyxml2::XMLElement* l_nPoint = _nNode->FirstChildElement("technique_common")->FirstChildElement("point");
    if (l_nPoint) {
        glm::vec4 cor = getColor(l_nPoint);
        return std::make_tuple(cor, Chimera::LightType::POSITIONAL);
    }

    l_nPoint = _nNode->FirstChildElement("technique_common")->FirstChildElement("directional");
    if (l_nPoint != nullptr) {
        glm::vec4 cor = getColor(l_nPoint);
        return std::make_tuple(cor, Chimera::LightType::DIRECTIONAL);
    }

    return std::make_tuple(glm::vec4(), Chimera::LightType::POSITIONAL);
}
} // namespace Chimera