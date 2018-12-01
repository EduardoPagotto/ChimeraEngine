#include "chimera/loader/LibraryImages.hpp"
#include "chimera/core/ExceptionChimera.hpp"

#include <tuple>

namespace ChimeraLoaders {

LibraryImages::LibraryImages(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryImages::~LibraryImages() {}

std::tuple<std::string, std::string> LibraryImages::target() {

    tinyxml2::XMLElement* l_nImgs = root->FirstChildElement("library_images")->FirstChildElement("image");
    for (l_nImgs; l_nImgs; l_nImgs = l_nImgs->NextSiblingElement()) {

        std::string l_id = l_nImgs->Attribute("id");
        if (url.compare(l_id) == 0) {

            tinyxml2::XMLElement* l_nTec = l_nImgs->FirstChildElement("init_from");
            if (l_nTec != nullptr) {
                std::string pathFileImage = l_nTec->GetText();
                return std::make_tuple(l_id, pathFileImage);
            }
        }
    }

    throw Chimera::ExceptionChimera("Imagen não encontrada:" + url);
}
} // namespace ChimeraLoaders