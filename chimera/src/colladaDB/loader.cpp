#include "chimera/colladaDB/loader.hpp"
#include "SDL2/SDL.h"

namespace Chimera {

const pugi::xml_node colladaGetLibrary(const pugi::xml_node& node, const std::string& instance, const std::string key) {

    std::string test = std::string("library_" + instance + "s");
    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {
        std::string name = n.name();
        if (name == test) {
            SDL_Log("%s encontrado", test.c_str());
            for (pugi::xml_node t = n.first_child(); t; t = t.next_sibling()) {

                pugi::xml_attribute attr = t.attribute("id");

                // for (pugi::xml_attribute attr = person.first_attribute(); attr; attr = attr.next_attribute())
                // {
                //     std::cout << " " << attr.name() << "=" << attr.value() << std::endl;
                // }

                std::string id = attr.value();
                if (id == key) {
                    SDL_Log("Elemento %s", t.name());
                    return t;
                }
            }
        }
    }

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s não encontrado key: %s", test.c_str(), key.c_str());
    return pugi::xml_node();
}

InstanceCollada* colladaURL(InstanceCollada* handle, const std::string& instance, const std::string& url) {

    const char* urlFile = "file://";
    size_t urlFileLen = strlen(urlFile);
    std::size_t found = url.find(urlFile, 0, urlFileLen);

    if (found != std::string::npos) {

        std::string temp = url.substr(urlFileLen, std::string::npos); //"file://./assets/models/piso2_mestre.xml#Scene"

        std::size_t mark1 = temp.rfind("#"); // FIXME: criar erro se nao achar!!!!
        if (mark1 == std::string::npos) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "URL %s não encontrado: #", temp.c_str());
            return nullptr;
        }

        std::string key = temp.substr(mark1 + 1, std::string::npos);
        std::string target = url.substr(urlFileLen, mark1);
        InstanceCollada* ret = new InstanceCollada;

        pugi::xml_parse_result result = ret->doc.load_file(target.c_str());

        if (result.status != pugi::status_ok) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Arquivo %s não encontrado", target.c_str());
            return nullptr;
        }

        SDL_Log("Novo Arquivo carregado %s, Key: %s, Status: %s", target.c_str(), key.c_str(), result.description());
        ret->root = ret->doc.child("COLLADA");
        ret->node = colladaGetLibrary(ret->root, instance, key);
        return ret;

    } else {

        std::size_t found = url.rfind("#");
        std::string key = (found != std::string::npos) ? url.substr(found + 1, std::string::npos) : url;
        handle->node = colladaGetLibrary(handle->root, instance, key);

        return nullptr;
    }
}
} // namespace Chimera