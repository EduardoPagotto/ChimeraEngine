#include "chimera/colladaDB/loader.hpp"
#include "SDL2/SDL.h"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/Transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

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

void loadImage(InstanceCollada* handle, const std::string& id) {

    InstanceCollada* i = colladaURL(handle, "image", id);
    pugi::xml_node node = (i == nullptr) ? handle->node : i->node;
    pugi::xml_node init = node.child("init_from");
    if (init != nullptr) {
        pugi::xml_text pathFile = init.text();
        if (pathFile != nullptr) {
            std::string f = pathFile.as_string();
            SDL_Log("Nova textura %s, Key: %s", f.c_str(), id.c_str());
        }
    }
}
//---
void textSplit(const std::string& sIn, std::vector<std::string>& vOut, char delimiter) { // TODO: ler como matrix!!!!!!
    std::string token;
    std::istringstream tokenStream(sIn);
    while (std::getline(tokenStream, token, delimiter))
        vOut.push_back(token);
}

void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat) {
    std::vector<std::string> textData;
    textSplit(text, textData, ' ');
    for (const std::string& val : textData)
        arrayFloat.push_back(std::stod(val));
}

glm::mat4 floatArrayToMat4(const std::vector<float>& listaMatrix) {

    if (listaMatrix.size() != 16)
        throw std::string("Tamanho da Matrix invalido" + std::to_string(listaMatrix.size()));

    float ponteiroFloat[16];
    int indice = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int pos = i + (4 * j);
            ponteiroFloat[pos] = listaMatrix[indice];
            indice++;
        }
    }

    return glm::make_mat4(&ponteiroFloat[0]);
}

void loadNode(InstanceCollada* handle, Registry* reg) {

    Entity entity = reg->createEntity(handle->node.attribute("name").value(), handle->node.attribute("id").value());
    for (pugi::xml_node n = handle->node.first_child(); n; n = n.next_sibling()) {

        std::string val = n.name();
        if (val == "matrix") {
            std::string sid = n.attribute("sid").value();
            if (sid == "transform") {

                std::vector<float> arrayFloat;
                textToFloatArray(n.text().as_string(), arrayFloat);
                glm::mat4 transLocal = floatArrayToMat4(arrayFloat);

                ComponentTrans tc = entity.addComponent<ComponentTrans>(new Transform(transLocal));
                // tc.trans = new Transform(transLocal);
            }
        } else if (val == "instance_geometry") {

            InstanceCollada* novo = colladaURL(handle, "geometrie", n.attribute("url").value());
            if (novo != nullptr)
                handle = novo;
        }
    }
}

void loadAll(InstanceCollada* handle, Registry* reg) {

    for (pugi::xml_node n = handle->node.first_child(); n; n = n.next_sibling()) {
        handle->node = n;
        loadNode(handle, reg);
    }
}

} // namespace Chimera