#include "chimera/colladaDB/ColladaImage.hpp"
#include "chimera/core/visible/TextureManager.hpp"

namespace Chimera {

ColladaImage::~ColladaImage() {
    mapaTex.clear();
    mapa2D.clear();
}

void ColladaImage::loadImage(const std::string& id) {

    pugi::xml_node node = getLibrary("library_images", id);
    pugi::xml_node init = node.child("init_from");
    if (init != nullptr) {
        pugi::xml_text pathFile = init.text();
        if (pathFile != nullptr) {
            std::string f = pathFile.as_string();
            SDL_Log("Nova textura %s, Key: %s", f.c_str(), id.c_str());
            TextureManager::loadFromFile(id, f, TexParam());
            return;
        }
    }

    throw std::string("Textura nao encontrada: " + id);
}

void ColladaImage::create(pugi::xml_node nodeParent) {

    for (pugi::xml_node param = nodeParent.first_child(); param; param = param.next_sibling()) {

        std::string sProf = param.name();
        std::string sid = param.attribute("sid").value();
        if (sProf == "newparam") {

            pugi::xml_node val1 = param.first_child();
            std::string sVal1 = val1.name();

            if (sVal1 == "surface") {

                std::string keyImage = val1.child("init_from").text().as_string();
                loadImage(keyImage);
                mapaTex[sid] = keyImage;

            } else if (sVal1 == "sampler2D") {

                std::string keyMap = val1.child("source").text().as_string();
                mapa2D[sid] = keyMap;
            }
        }
    }
}

Texture* ColladaImage::getTexureByName(const std::string& name) {
    std::string idTex = mapaTex[mapa2D[name]];
    return TextureManager::get(idTex);
}
} // namespace Chimera