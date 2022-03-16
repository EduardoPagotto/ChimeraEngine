#include "LibraryImages.hpp"

#include <tuple>

namespace Chimera {

std::tuple<std::string, std::string> LibraryImages::target() {

    for (tinyxml2::XMLElement* l_nImgs = root->FirstChildElement("library_images")->FirstChildElement("image"); l_nImgs;
         l_nImgs = l_nImgs->NextSiblingElement()) {

        std::string l_id = l_nImgs->Attribute("id");
        if (url.compare(l_id) == 0) {

            tinyxml2::XMLElement* l_nTec = l_nImgs->FirstChildElement("init_from");
            if (l_nTec != nullptr) {
                std::string pathFileImage = l_nTec->GetText();
                return std::make_tuple(l_id, pathFileImage);
            }
        }
    }

    throw std::string("Imagen não encontrada:" + url);
}
} // namespace Chimera