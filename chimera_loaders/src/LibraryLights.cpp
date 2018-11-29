#include "LibraryLights.hpp"
#include "ExceptionChimera.hpp"

namespace ChimeraLoaders {

LibraryLights::LibraryLights(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryLights::~LibraryLights() {}

Chimera::Light* LibraryLights::target() {

    tinyxml2::XMLElement* l_nLight = root->FirstChildElement("library_lights")->FirstChildElement("light");
    for (l_nLight; l_nLight; l_nLight = l_nLight->NextSiblingElement()) {

        std::string l_id = l_nLight->Attribute("id");
        if (url.compare(l_id) == 0) {

            Chimera::Light* pLight = new Chimera::Light(nullptr, l_id);

            auto ret_data = loadDiffuseLightColor(l_nLight);
            pLight->setDiffuse(std::get<0>(ret_data));
            pLight->setType(std::get<1>(ret_data));

            return pLight;
        }
    }

    throw Chimera::ExceptionChimera("Light nao encontrada: " + url);
}

Chimera::Color LibraryLights::getColor(tinyxml2::XMLElement* l_nColorVal) {

    std::vector<float> l_arrayF;
    const char* l_val = l_nColorVal->FirstChildElement("color")->GetText();
    loadArrayBtScalar(l_val, l_arrayF);
    return Chimera::Color(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);
}

std::tuple<Chimera::Color, Chimera::LightType> LibraryLights::loadDiffuseLightColor(tinyxml2::XMLElement* _nNode) {
    tinyxml2::XMLElement* l_nPoint = _nNode->FirstChildElement("technique_common")->FirstChildElement("point");
    if (l_nPoint) {
        Chimera::Color cor = getColor(l_nPoint);
        return std::make_tuple(cor, Chimera::LightType::POSITIONAL);
    }

    l_nPoint = _nNode->FirstChildElement("technique_common")->FirstChildElement("directional");
    if (l_nPoint != nullptr) {
        Chimera::Color cor = getColor(l_nPoint);
        return std::make_tuple(cor, Chimera::LightType::DIRECTIONAL);
    }
}

} // namespace ChimeraLoaders