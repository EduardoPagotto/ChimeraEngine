#include "chimera/colladaDB/loader.hpp"
#include "SDL2/SDL.h"

namespace Chimera {
LoadColladaURL::LoadColladaURL(const std::string& url) {

    const char* urlFile = "file://";
    size_t urlFileLen = strlen(urlFile);

    std::size_t found = url.find(urlFile, 0, urlFileLen);
    if (found != std::string::npos) {

        std::string temp = url.substr(urlFileLen, std::string::npos); //"file://./assets/models/piso2_mestre.xml#scene"

        std::size_t mark1 = temp.rfind("#");

        std::string startElement = temp.substr(mark1 + 1, std::string::npos);
        std::string target = url.substr(urlFileLen, mark1);

        SDL_Log("Novo Arquivo carregado %s, Node: %s", target.c_str(), startElement.c_str());

        pugi::xml_parse_result result = doc.load_file(target.c_str());

        root = doc.child("COLLADA");

        pugi::xml_node node = root.child(startElement.c_str()); // scene

        for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) { // childs!!
            SDL_Log("Elemento %s", n.name());
        }

        // for (pugi::xml_attribute attr = person.first_attribute(); attr; attr = attr.next_attribute())
        // {
        //     std::cout << " " << attr.name() << "=" << attr.value() << std::endl;
        // }

        SDL_Log("Parse %s, Node: %s", startElement.c_str(), result.description());
    }
}

LoadColladaURL::~LoadColladaURL() {}
} // namespace Chimera